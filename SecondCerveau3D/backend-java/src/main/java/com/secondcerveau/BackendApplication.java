package com.secondcerveau;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;

@SpringBootApplication
@ComponentScan(basePackages = {"com.secondcerveau"})
public class BackendApplication {
    
    public static void main(String[] args) {
        SpringApplication.run(BackendApplication.class, args);
        System.out.println("\n=== SecondCerveau3D Backend Started ===");
        System.out.println("Agent Orchestration Service: http://localhost:8080");
        System.out.println("WebSocket endpoint: ws://localhost:8080/ws\n");
    }
}
