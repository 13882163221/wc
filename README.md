# C 语言模式 – Part 2

著 Adam Petersen <adam@adampetersen.se>

<!-- Every non-trivial program passes through a number of different states during its lifecycle. Describing this lifecycle as a finite state  machine  is  a  simple  and  useful  abstraction.  In  this  part  of  the  series,  we  will  investigate  different  strategies  for  implementing state machines. The goal is to identify mechanisms that let the code communicate the intent of expressing the problem as a finite state machine. -->
每个大型程序在其生命周期中都会经历许多不同的状态。将此生命周期描述为有限状态机是一种简单而有用的抽象。在系列的这一部分中，我们将研究实现状态机的不同策略。目标是确定让代码传达将问题表达为有限状态机的意图的机制。

<!-- Traditional Solution with Conditionals -->
## __使用条件的传统解决方案__

<!-- Consider a simple, digital stop-watch. In its most basic version, it has two states: started and stopped. A traditional and direct way to implement this behavior in C is with conditional logic in the shape of switch/case statements and/or if-else chains. -->
假设有一个简单的数字秒表。在最基本的版本中，它有两种状态： started 和 stopped。在 C 中实现此行为的一种传统且直接的方法是使用 switch/case 语句和(或) if-else 链形式的条件逻辑。

<!-- The digital stop-watch in this example is implemented as a First-Class ADT [1]. -->
本示例中的数字秒表是用 First-Class ADT [1] 实现的。

```c++
typedef enum
{
    stopped,
    started
} State;

struct DigitalStopWatch
{
    /* Let a variable hold the state of our object. */
    State state;
    TimeSource source;
    Display watchDisplay;
};

void startWatch(DigitalStopWatchPtr instance)
{
    switch (instance->state)
    {
        case started:
            /* Already started -> do nothing. */
            break;
        case stopped:
            instance->state = started;
            break;
        default:
            error("Illegal state");
            break;
    }
}

void stopWatch(DigitalStopWatchPtr instance)
{
    switch (instance->state)
    {
    case started:
        instance->state = stopped;
        break;
    case stopped:
        /* Already stopped -> do nothing. */
        break;
    default:
        error("Illegal state");
        break;
    }
}
```

<!-- While this approach has the advantage of being simple and easy to understand, it introduces several potential problems: -->
虽然这种方法具有简单易懂的优点，但它引入了几个潜在的问题：

<!-- 1.It  doesn't  scale.  In  large  state  machines  the  code  may  stretch  over  page  after  page  of  nested  conditional  logic.  Imagine the true maintenance nightmare of changing large, monolithic segments of conditional statements. -->
1. *它无法扩展*。在大型状态机中，代码可能会因嵌套条件逻辑延展很多段。想象一下真正的维护噩梦，更改条件语句的大型单体段。

    <!-- 2.Duplication. The conditional logic tends to be repeated, with small variations, in all functions that access the state variable. As always, duplication leads to error-prone maintenance. For example, simply adding a new state implies changing several functions. -->
2. *重复*。在访问状态变量的所有函数中，条件逻辑往往会重复，变化很小。通常，重复会导致容易出错的维护。例如，简单地添加一个新状态却更改多个函数。

    <!-- 3.No  separation  of  concerns.  When  using  conditional  logic  for  implementing  state  machines,  there  is  no  clear  separation  between  the  code  of  the  state  machine  itself  and  the  actions  associated  with  the  various  events.  This  makes  the  code  hide  the  original  intent  (abstracting  the  behaviour  as  a  finite  state  machine)  and  thus  making  the  code less readable. -->
3. *没有关注点分离*。当使用条件逻辑来实现状态机时，状态机本身的代码和与各种事件相关的动作之间没有明确的分离。这使代码违背了原始意图（将行为抽象为有限状态机），从而降低代码的可读性。

<!-- A Table-based Solution -->
## __基于表的解决方案__

<!-- The  second  traditional  approach  to  implement  finite  state  machines  is  through  transition  tables.  Using  this  technique,  our  original example now reads as follows.  -->
实现有限状态机的第二种传统方法是通过转换表。使用这种技术，我们的原始示例现在如下所示。

```c++
typedef enum
{
    stopped,
    started
} State;

typedef enum
{
    stopEvent,
    startEvent
} Event;

#define NO_OF_STATES 2
#define NO_OF_EVENTS 2

static State TransitionTable[NO_OF_STATES][NO_OF_EVENTS] = {
    {stopped, started},
    {stopped, started}};

void startWatch(DigitalStopWatchPtr instance)
{
    const State currentState = instance->state;
    instance->state = TransitionTable[currentState][startEvent];
}
void stopWatch(DigitalStopWatchPtr instance)
{
    const State currentState = instance->state;
    instance->state = TransitionTable[currentState][stopEvent];
}
```

<!-- The choice of a transition table over conditional logic solved the previous problems: -->
选择转移表相比条件逻辑解决了之前的问题：

<!-- 1.Scales  well.  Independent  of  the  size  of  the  state  machine,  the  code  for  a  state  transition  is  just  one,  simple  table-lookup.
2.No duplication. Without the burden of repetitive switch/case statements, modification comes easily. When adding a new state, the change is limited to the transition table; all code for the state handling itself goes unchanged.
3.Easy to understand. A well structured transition table serves as a good overview of the complete lifecycle. -->

1. *缩放良好*。独立于状态机的大小，状态转换的代码只是一个简单的表查找。
2. *没有重复*。没有重复 switch/case 语句的负担，修改很容易。添加新状态时，更改仅限于转换表；状态处理本身的所有代码都保持不变。
3. *容易明白*。结构良好的转换表可以很好地概述整个生命周期。

<!-- Shortcomings of Tables -->
## __表的缺点__

<!-- As appealing as table-based state machines may seem at first, they have a major drawback: it is very hard to add actions to the transitions defined in the table. For example, the watch would typically invoke a function that starts to tick millisecondsupon a transition to state started. As the state transition isn't explicit, conditional logic has to be added in order to ensure that the tick-function is invoked solely as the transition succeeds. In combination with conditional logic, the initial benefits of the table-based solution soon decrease together with the quality of the design. -->
乍一看，基于表的状态机看起来很吸引人，但它们有一个主要缺点：很难在表状态转换时添加操作。例如，手表通常会调用一个函数，该函数在转换到开始状态时开始以毫秒为单位计时。由于状态转换不是明确的，因此必须添加条件逻辑以确保仅在转换成功时调用刻度函数（译注：即只在 `if (stopped --> started)` 时才调用函数）。使用了条件逻辑，基于表的解决方案的初始优势很快就会随着设计质量的降低而减少。

<!-- Other  approaches  involve  replacing  the  simple  enumerations  in  the  table  with  pointers  to  functions  specifying  the  entry  actions. Unfortunately, the immediate hurdle of trying to map state transitions to actions in a table based solution is that the functions  typically  need  different  arguments.  This  problem  is  possible  to  solve,  but  the  resulting  design  looses,  in  my  opinion,  both  in  readability  as  well  as  in  cohesion  as  it  typically  implies  either  giving  up  on  type  safety  or  passing  around  unused parameters. None of these alternatives seem attractive.  -->
其他方法包括用指向指定入口动作的函数的指针替换表中的简单枚举（译注：我们的代码大多基于此方法）。不幸的是，在基于表的解决方案中尝试将状态转换映射到操作的直接障碍是，函数通常需要不同的参数。这个问题是有可能解决的，但在我看来，由此产生的设计在可读性和内聚性方面都很松散，因为它通常意味着要么放弃类型安全，要么传递未使用的参数。这些替代品似乎都没有吸引力。（译注：我认为最严重的问题是类型安全，不只是状态机中，读写越界经常发生）

<!-- Transition tables definitely have their use, but when actions have to be associated with state transitions, the STATE pattern provides a better alternative.  -->
转换表肯定有它们的用途，但是当动作必须与状态转换相关联时，STATE 模式提供了更好的选择。

<!-- Enter STATE Pattern -->

## __走进 STATE 模式__

<!-- In  its  description  of  the  STATE  pattern,  Design  Patterns  [2]  defines  the  differences  from  the  table-based  approach  as  "the STATE  pattern  models  state-specific  behavior,  whereas  the  table-driven  approach  focuses  on  defining  state  transitions". When applying the STATE pattern to our example, the structure in Figure 1 emerges. -->
在对 STATE 模式的描述中，《设计模式》 [2] 将与基于表的方法的区别定义为“*状态模式建模特定于状态的行为，而表驱动的方法侧重于定义状态转换*”。将 STATE 模式应用于我们的示例时，会出现图 1 中的结构。

<div align=center><img src="./pic/2-1.png"></div>
<div align=center>图1：状态模式结构</div>

<!-- This diagram definitely looks like an object oriented solution. But please don't worry -- we will not follow the temptation of the  dark  side  and  emulate  inheritance  in  C.  However,  before  developing  a  concrete  implementation,  let's  explain  the  involved participants. -->
这个图看起来确实像一个面向对象的解决方案。但是请不要担心——我们不会跟随黑暗面的诱惑在 C 中模拟继承。 但是，在开发具体实现之前，让我们解释一下所涉及的参与者。

<!-- •DigitalStopWatch: Design Patterns [2] defines this as the context. The context has a reference to one o
