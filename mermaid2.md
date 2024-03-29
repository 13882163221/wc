```mermaid
    sequenceDiagram
     autonumber
     loop
    MANO->>SCF: Post-Scale-Out
    SCF->>SCF: Determine Node/Pod
    SCF->>MANO: Rebuild Pod
    MANO->>SCF: ACK
    SCF->>SCF: Wait Pod Ready
    loop VNFC Only 
    SCF->>NPS: DSP OMU
    NPS->>SCF: Result
    end
    SCF->>MANO: Scale Pod
    end
```
  
```mermaid
gantt
dateFormat s.SSS
axisFormat %S.%L
title PC Chrome 加载过程图
点击订单页 : milestone, m1, 0.000,0
加载HTML:active,m2,after m1,230ms
加载CSS:crit,m3,after m2,1720ms
渲染样式和出现loading:active,crit,m4,after m3,1050ms
加载JS:crit,m5,after m4,2000ms
解析JS:active,m6,after m5,240ms
查询主接口:active,m7,after m6,300ms
加载文字内容:active,m8,after m7,560ms
加载图片内容:active,m9,after m8,600ms
首屏完成 : milestone, m100, 6.800, 0
```

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

```mermaid

graph LR;

A(进程启动) -->B(组装启动参数) -->C(根据启动参数启动Worker进程) -->D((开始轮询)) -->E(读取Worker进程的消息) -->F{是否有消息}

F -- Yes --> G(处理消息: 重启所有服务或只重启名字/通知服务) -->H(检查子进程状态)

F -- No --> HH --> I{进程状态是否正常}

I -- Yes --> DI -- No --> J(拉起子进程) --> D

```
