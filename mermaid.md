 # Flow chart
 ```mermaid
	 graph TD
      A[Start] --> B;
      B -- Yes --> C[OK];
      C --> D[Rethink];
      D --> B;
      B -- No --> E[End]; 
``` 
# Sequence chart 
 ```mermaid
	 sequenceDiagram
      Alice->>John: Hello John, how are you?
      John-->>Alice: Great! 
``` 
# Gantt chart 
 ```mermaid
	 gantt
      dateFormat  DD/MM/YY
      Task 1 :active, 09/04/18, 30/10/20
      Milestone 1 :milestone, 29/09/20, 1d
      Task 2 :active, 11/11/19, 19/10/20
      Milestone 2 :milestone, 30/09/20, 1d
      Task 3 :active, 18/02/19, 10/07/20
      Milestone 3 :milestone, 01/10/20, 1d
      Task 4 :active, 11/03/19, 04/08/20
      Milestone 4 :milestone, 11/10/19, 1d 
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


