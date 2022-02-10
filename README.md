```mermaid
    graph TD
用户-->生成方,如api
生成方,如api --> KAFAKA
KAFAKA --> 消费方 & 合作方
KAFAKA --"<<Deploy>>"--- 客户方
客户方 --> 用户提供地址
```
