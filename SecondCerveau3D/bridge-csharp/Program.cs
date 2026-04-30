using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace SecondCerveau.Bridge {
    
    public class NativeOrchestratorBridge {
        /* P/Invoke declarations for native C functions */
        [DllImport("libagent_orchestrator.so", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_orchestrator_context();
        
        [DllImport("libagent_orchestrator.so", CallingConvention = CallingConvention.Cdecl)]
        private static extern void destroy_orchestrator_context(IntPtr context);
        
        [DllImport("libagent_orchestrator.so", CallingConvention = CallingConvention.Cdecl)]
        private static extern int execute_agent_task(IntPtr context, string agent_type, string task_json);
        
        [DllImport("libagent_orchestrator.so", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr get_agent_result(IntPtr context, int task_id);
        
        [DllImport("libfilescanner.so", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr scan_directory(string path);
        
        private IntPtr _context;
        private Dictionary<int, AgentTask> _tasks;
        
        public NativeOrchestratorBridge() {
            _context = create_orchestrator_context();
            _tasks = new Dictionary<int, AgentTask>();
            Console.WriteLine("[C# Bridge] Initialized native orchestrator context");
        }
        
        public void Dispose() {
            destroy_orchestrator_context(_context);
        }
        
        public int SubmitAgentTask(string agentType, Dictionary<string, object> taskData) {
            string json = JsonConvert.SerializeObject(taskData);
            int taskId = execute_agent_task(_context, agentType, json);
            
            _tasks[taskId] = new AgentTask {
                Id = taskId,
                Type = agentType,
                Timestamp = DateTime.Now,
                Status = "SUBMITTED"
            };
            
            Console.WriteLine($"[C# Bridge] Submitted {agentType} task: {taskId}");
            return taskId;
        }
        
        public string GetTaskResult(int taskId) {
            IntPtr resultPtr = get_agent_result(_context, taskId);
            
            if (resultPtr != IntPtr.Zero) {
                string result = Marshal.PtrToStringAnsi(resultPtr);
                if (_tasks.ContainsKey(taskId)) {
                    _tasks[taskId].Status = "COMPLETED";
                }
                return result;
            }
            
            return null;
        }
        
        public List<AgentTask> GetPendingTasks() {
            List<AgentTask> pending = new List<AgentTask>();
            foreach (var task in _tasks.Values) {
                if (task.Status == "SUBMITTED") {
                    pending.Add(task);
                }
            }
            return pending;
        }
        
        public void ExecuteScan(string path) {
            var taskData = new Dictionary<string, object> {
                { "path", path },
                { "type", "filesystem_scan" }
            };
            
            SubmitAgentTask("blue_monitor", taskData);
        }
    }
    
    public class AgentTask {
        public int Id { get; set; }
        public string Type { get; set; }
        public DateTime Timestamp { get; set; }
        public string Status { get; set; }
    }
    
    public class Program {
        static void Main(string[] args) {
            Console.WriteLine("=== SecondCerveau3D - C# Bridge ===");
            Console.WriteLine("Connecting to native agent orchestrator...\n");
            
            using (var bridge = new NativeOrchestratorBridge()) {
                /* Example: Submit reconnaissance task */
                var reconTask = new Dictionary<string, object> {
                    { "target", "localhost" },
                    { "ports", new[] { 22, 80, 443, 8080 } },
                    { "timeout", 5000 }
                };
                
                int taskId = bridge.SubmitAgentTask("red_recon", reconTask);
                Console.WriteLine($"\nTask submitted with ID: {taskId}");
                
                /* Example: Submit monitoring task */
                bridge.ExecuteScan("/tmp");
                
                /* Wait for results */
                System.Threading.Thread.Sleep(2000);
                
                Console.WriteLine("\nPending tasks:");
                foreach (var task in bridge.GetPendingTasks()) {
                    Console.WriteLine($"  - Task {task.Id}: {task.Type} ({task.Status})");
                }
            }
        }
    }
}
