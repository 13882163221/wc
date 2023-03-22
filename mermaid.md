```mermiad
graph LR
	id1["[xxx]"]
	id2[["[[xxx]]"]]
	id3(["([xxx])"])
	id4[("[(xxx)]")]
	id5(("((xxx))"))
	id6>">xxx]"]
	id7{"{xxx}"}
	id8{{"{{xxx}}"}}
	id9[/"[/xxx/]"/]
	id10[/"[/xxx\]"\]
	id11[\"\xxx/"/]
	id12[more]
	id1 --- |"---"| id2 -->|"-->"| id3 -.->|"-.->"| id4
	id5 --- |直线| id6 -->|带箭头<br/>的直线| id7 -.->|带箭头<br/>的虚线| id8
	id9 == "==>" ==>id10
	id9 -.- |"-.-"| id10
	id10 --> id11 -.- id12
```
