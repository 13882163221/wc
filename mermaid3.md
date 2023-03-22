```mermaid
flowchart
subgraph CORBANBI
subgraph CorbaAgent
Agent_CORBA-进程 --> Corba_Service-进程
Agent_CORBA-进程 --> Naming_Service-进程
Agent_CORBA-进程 --> Notify_Service-进程
end
end
```
