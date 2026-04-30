import React, { useEffect, useRef } from 'react';
import * as THREE from 'three';

const Graph3D = () => {
    const containerRef = useRef(null);
    const sceneRef = useRef(null);
    const cameraRef = useRef(null);
    const rendererRef = useRef(null);
    const nodesRef = useRef([]);
    const edgesRef = useRef([]);

    useEffect(() => {
        initializeScene();
        loadGraph();

        return () => {
            // Cleanup
            if (rendererRef.current) {
                rendererRef.current.dispose();
            }
        };
    }, []);

    const initializeScene = () => {
        // Scene setup
        const scene = new THREE.Scene();
        scene.background = new THREE.Color(0x0a0e27);

        // Camera setup
        const camera = new THREE.PerspectiveCamera(
            75,
            containerRef.current.clientWidth / containerRef.current.clientHeight,
            0.1,
            1000
        );
        camera.position.set(0, 0, 30);

        // Renderer setup
        const renderer = new THREE.WebGLRenderer({ antialias: true });
        renderer.setSize(
            containerRef.current.clientWidth,
            containerRef.current.clientHeight
        );
        containerRef.current.appendChild(renderer.domElement);

        // Lighting
        const light1 = new THREE.DirectionalLight(0xffffff, 0.8);
        light1.position.set(10, 20, 10);
        scene.add(light1);

        const light2 = new THREE.AmbientLight(0x404040, 0.6);
        scene.add(light2);

        // Store references
        sceneRef.current = scene;
        cameraRef.current = camera;
        rendererRef.current = renderer;

        // Animation loop
        const animate = () => {
            requestAnimationFrame(animate);

            // Rotate nodes
            nodesRef.current.forEach(node => {
                node.mesh.rotation.x += 0.001;
                node.mesh.rotation.y += 0.002;
            });

            renderer.render(scene, camera);
        };
        animate();

        // Handle window resize
        window.addEventListener('resize', () => {
            const width = containerRef.current.clientWidth;
            const height = containerRef.current.clientHeight;
            camera.aspect = width / height;
            camera.updateProjectionMatrix();
            renderer.setSize(width, height);
        });
    };

    const loadGraph = async () => {
        try {
            const graphData = await window.electronAPI?.getGraph();
            
            if (graphData) {
                addNodesToScene(graphData.nodes);
                addEdgesToScene(graphData.edges);
            }
        } catch (error) {
            console.error('Failed to load graph:', error);
        }
    };

    const addNodesToScene = (nodes) => {
        const scene = sceneRef.current;

        nodes.forEach(node => {
            const geometry = new THREE.SphereGeometry(1, 32, 32);
            
            // Color based on threat level
            let color = 0x00ff00; // Green
            if (node.threat >= 7) color = 0xff0000; // Red
            else if (node.threat >= 4) color = 0xffaa00; // Orange
            
            const material = new THREE.MeshPhongMaterial({ color });
            const mesh = new THREE.Mesh(geometry, material);

            // Random position
            mesh.position.set(
                Math.random() * 20 - 10,
                Math.random() * 20 - 10,
                Math.random() * 20 - 10
            );

            scene.add(mesh);

            // Add label
            const canvas = document.createElement('canvas');
            const ctx = canvas.getContext('2d');
            ctx.fillStyle = '#fff';
            ctx.font = 'bold 20px Arial';
            ctx.textAlign = 'center';
            ctx.fillText(node.label, canvas.width / 2, canvas.height / 2);

            const texture = new THREE.CanvasTexture(canvas);
            const labelMaterial = new THREE.MeshBasicMaterial({ map: texture });
            const labelGeometry = new THREE.PlaneGeometry(2, 1);
            const label = new THREE.Mesh(labelGeometry, labelMaterial);
            label.position.copy(mesh.position);
            label.position.z += 1.5;
            scene.add(label);

            nodesRef.current.push({ id: node.id, mesh, label });
        });
    };

    const addEdgesToScene = (edges) => {
        const scene = sceneRef.current;

        edges.forEach(edge => {
            const sourceNode = nodesRef.current.find(n => n.id === edge.source);
            const targetNode = nodesRef.current.find(n => n.id === edge.target);

            if (sourceNode && targetNode) {
                const geometry = new THREE.BufferGeometry();
                const positions = new Float32Array([
                    sourceNode.mesh.position.x, sourceNode.mesh.position.y, sourceNode.mesh.position.z,
                    targetNode.mesh.position.x, targetNode.mesh.position.y, targetNode.mesh.position.z
                ]);
                geometry.setAttribute('position', new THREE.BufferAttribute(positions, 3));

                let color = 0x00d4ff;
                if (edge.type === 'threat') color = 0xff0000;

                const material = new THREE.LineBasicMaterial({ color });
                const line = new THREE.Line(geometry, material);
                scene.add(line);

                edgesRef.current.push(line);
            }
        });
    };

    return (
        <div ref={containerRef} style={{
            width: '100%',
            height: '100%',
            position: 'relative'
        }}>
            <div style={{
                position: 'absolute',
                top: '10px',
                left: '10px',
                color: '#00d4ff',
                fontSize: '12px',
                backgroundColor: 'rgba(0,0,0,0.5)',
                padding: '10px',
                borderRadius: '4px',
                zIndex: 10
            }}>
                🟢 Safe  🟠 Medium  🔴 Critical<br/>
                Total Nodes: {nodesRef.current.length}<br/>
                Total Connections: {edgesRef.current.length}
            </div>
        </div>
    );
};

export default Graph3D;
