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

```c
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

<!-- 1.It  doesn't  scale.  In  large  state  machines  the  code  may  stretch  over  page  after  page  of  nested  conditional  logic.  Imagine the true maintenance nightmare of changing large, monolithic segments of conditional statements.
2.Duplication. The conditional logic tends to be repeated, with small variations, in all functions that access the state variable. As always, duplication leads to error-prone maintenance. For example, simply adding a new state implies changing several functions.
3.No  separation  of  concerns.  When  using  conditional  logic  for  implementing  state  machines,  there  is  no  clear  separation  between  the  code  of  the  state  machine  itself  and  the  actions  associated  with  the  various  events.  This  
makes  the  code  hide  the  original  intent  (abstracting  the  behaviour  as  a  finite  state  machine)  and  thus  making  the  code less readable. -->

1. *它无法扩展*。在大型状态机中，代码可能会因嵌套条件逻辑延展很多段。想象一下真正的维护噩梦，更改条件语句的大型单体段。

2. *重复*。在访问状态变量的所有函数中，条件逻辑往往会重复，变化很小。通常，重复会导致容易出错的维护。例如，简单地添加一个新状态却更改多个函数。

3. *没有关注点分离*。当使用条件逻辑来实现状态机时，状态机本身的代码和与各种事件相关的动作之间没有明确的分离。这使代码违背了原始意图（将行为抽象为有限状态机），从而降低代码的可读性。

<!-- A Table-based Solution -->
## __基于表的解决方案__

<!-- The  second  traditional  approach  to  implement  finite  state  machines  is  through  transition  tables.  Using  this  technique,  our  original example now reads as follows.  -->
实现有限状态机的第二种传统方法是通过转换表。使用这种技术，我们的原始示例现在如下所示。

```c
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
乍一看，基于表的状态机看起来很吸引人，但它们有一个主要缺点：很难在表状态转换时添加操作。例如，手表通常会调用一个函数，该函数在转换到状态开始时开始以毫秒为单位计时。由于状态转换不是明确的，因此必须添加条件逻辑以确保仅在转换成功时调用刻度函数（译注：即只在 stopped --> started 时才调用函数）。使用了条件逻辑，基于表的解决方案的初始优势很快就会随着设计质量的降低而减少。

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

<!-- •DigitalStopWatch: Design Patterns [2] defines this as the context. The context has a reference to one of our concrete states, without knowing exactly which one. It is the context that specifies the interface to the clients.
•WatchState: Defines the interface of the state machine, specifying all supported events.
•StoppedState and StartedState: These are concrete states and each one of them encapsulates the behavior associated with the state it represents. -->

* **DigitalStopWatch**：《设计模式》[2] 将其定义为上下文。上下文引用了我们的一种具体状态，但不知道确切是哪一种。它是指定客户端接口的上下文。
* **WatchState**：定义状态机的接口，指定所有支持的事件。
* **StoppedState** 和 **StartedState**：这些是具体的状态，它们中的每一个都封装了与其所代表的状态相关联的行为。

<!-- The  main  idea  captured  in  the  STATE  pattern  is  to  represent  each  state  as  an  object  of  its  own.  A  state  transition  simply  means changing the reference in the context (DigitalStopWatch) from one of the concrete states to the other.  -->
STATE 模式的主要思想是将每个状态表示为它自己的对象。状态转换仅意味着将上下文 (**DigitalStopWatch**) 中的引用从一种具体状态更改为另一种。

<!-- Implementation Mechanism -->

## __实现机制__

<!-- Which mechanism may be suitable for expressing this, clearly object oriented idea, in C? Returning to our example, we see that we basically have to switch functions upon each state transition. Luckily, the C language supplies one powerful feature, pointers to functions, that serves our needs perfectly by letting us change the behaviour of an object at run-time. Using this mechanism, the interface of the states would look as:  -->

哪种机制可能适合在 C 中表达这种清晰的面向对象的思想？回到例子，我们看到基本上必须在每次状态转换时切换函数。幸运的是，C 语言提供了一个强大的功能，即函数指针，它通过让我们在运行时更改对象的行为来完美地满足我们的需求。使用这种机制，状态的接口将如下所示：

<!-- Listing 1: The state interface in WatchState.h -->
### __清单 1：WatchState.h 中的状态接口__

```c
/* An incomplete type for the state representation itself. */
typedef struct WatchState *WatchStatePtr;

/* Simplify the code by using typedefs for the function pointers. */
typedef void (*EventStartFunc)(WatchStatePtr);
typedef void (*EventStopFunc)(WatchStatePtr);

struct WatchState
{
    EventStartFunc start;
    EventStopFunc stop;
};
```

<!-- Breaking the Dependency Cycle -->
## __打破依赖循环__

<!-- After getting used to the scary syntax of pointers to functions, the interface above looks rather pleasant. However, with the interface as it is, a dependency cycle will evolve.  -->
在习惯了可怕的函数指针语法之后，上面的接口看起来相当令人愉快。然而，随着接口的发展，会衍生依赖循环。

<!-- Consider  the  pointers  in  the  WatchState  structure.  Every  concrete  state  has  to  define  the  functions  to  be  pointed  at.  This  implies that each time an event is added to the interface, all concrete states have to be updated. The resulting code would be error-prone to maintain and not particularly flexible.  -->
考虑 **WatchState** 结构中的指针。每个具体状态都必须定义要指向的函数。这意味着每次将事件添加到接口时，都必须更新所有具体状态。生成的代码对维护来说容易出错，并且不是特别灵活。

<!-- The  good  news  is  that  breaking  this  dependency  cycle  is  simple  and  the  resulting  solution  has  the  nice  advantage  of  providing a potential error-handler. The trick is to provide a default implementation, as illustrated in the listing below.  -->
好消息是打破这种依赖循环很简单，并且最终的解决方案还有一个好处，提供潜在错误处理程序。诀窍是提供一个默认实现，如下面的清单所示。

<!-- Listing 2: Extend the interface in WatchState.h -->
### __清单 2：扩展 WatchState.h 中的接口__

```c
/* ..previous code as before.. */
void defaultImplementation(WatchStatePtr state);
```

<!-- Listing 3: Provide the default implementations in WatchState.c -->
### __清单 3：提供 WatchState.c 中的默认实现__

```c
static void defaultStop(WatchStatePtr state)
{
    /* We'll get here if the stop event isn't supported in the concrete state. */
}

static void defaultStart(WatchStatePtr state)
{
    /* We'll get here if the start event isn't supported in the concrete state. */
}

void defaultImplementation(WatchStatePtr state)
{
    state->start = defaultStart;
    state->stop = defaultStop;
}
```

<!-- Concrete States -->
## __具体状态__

<!-- The default implementation above completes the interface of the states. The interface of each state itself is minimal; all it has to do is to declare an entry function for the state.  -->
上面的默认实现完成了状态的接口。每个状态本身的接口是最小的；它所要做的就是为状态声明一个入口函数。

<!-- Listing 4: Interface of a concrete state, StoppedState.h -->
### __清单 4：具体状态的接口，StoppedState.h__

```c
#include "WatchState.h"
void transitionToStopped(WatchStatePtr state);
```

<!-- Listing 5: Interface of a concrete state, StartedState.h -->
### __清单 5：具体状态的接口，StartedState.h__

```c
#include "WatchState.h"
void transitionToStarted(WatchStatePtr state);
```

<!-- The  responsibility  of  the  entry  functions  is  to  set  the  pointers  in  the  passed  WatchState  structure  to  point  to  the  functions  specifying  the  behavior  of  the  particular  state.  As  we  can  utilize  the  default  implementation,  the  implementation  of  the  concrete states is straightforward; each concrete state only specifies the events of interest in that state.  -->
入口函数的职责是在传递的 **WatchState** 结构中设置指针以指向，定义了特定状态行为的函数。由于我们可以使用默认实现，因此具体状态的实现很简单；每个具体状态仅指定该状态中感兴趣的事件。

<!-- Listing 6: StoppedState.c -->
### __清单 6: StoppedState.c__

```c
#include "StoppedState.h"
/* Possible transition to the following state: */
#include "StartedState.h"

static void startWatch(WatchStatePtr state)
{
    transitionToStarted(state);
}

void transitionToStopped(WatchStatePtr state)
{
    /* Initialize with the default implementation before specifying
       the events to be handled in the stopped state. */
    defaultImplementation(state);
    state->start = startWatch;
}
```

<!-- Listing 7: StartedState.c -->
### __清单 7: StartedState.c__

```c
#include "StartedState.h"
/* Possible transition to the following state: */
#include "StoppedState.h"

static void stopWatch(WatchStatePtr state)
{
    transitionToStopped(state);
}

void transitionToStarted(WatchStatePtr state)
{
    /* Initialize with the default implementation before specifying
       the events to be handled in the started state. */
    defaultImplementation(state);
    state->stop = stopWatch;
}
```

<!-- Client Code -->
## 客户端代码

<!-- The reward for the struggle so far comes when implementing the context, i.e. the client of the state machine. All the client code has to do, after the initial state has been set, is to delegate the requests to the state.  -->
到目前为止，斗争的回报来自于实现上下文，即状态机的客户端。在设置初始状态之后，客户端代码要做的就是将请求委托给该状态。

```c
struct DigitalStopWatch
{
    struct WatchState state;
    TimeSource source;
    Display watchDisplay;
};

DigitalStopWatchPtr createWatch(void)
{
    DigitalStopWatchPtr instance = malloc(sizeof *instance);

    if (NULL != instance) {
        /* Set the initial state. */
        transitionToStopped(&instance->state);
        /* Initialize the other attributes here. */
    }
    return instance;
}

void destroyWatch(DigitalStopWatchPtr instance)
{
    free(instance);
}

void startWatch(DigitalStopWatchPtr instance)
{
    instance->state.start(&instance->state);
}

void stopWatch(DigitalStopWatchPtr instance)
{
    instance->state.stop(&instance->state);
}
```

<!-- A Debug Aid -->
## 调试帮助

<!-- In order to ease debugging, the state structure may be extended with a string holding the name of the actual state. Example:  -->
为了便于调试，状态结构可以用保存实际状态名称的字符串进行扩展。例子：

```c
void transitionToStopped(WatchStatePtr state) {
    defaultImplementation(state);
    state->name = "Stopped";
    state->start = startWatch;
}
```

<!-- Utilizing  this  extension,  it  becomes  possible  to  provide  an  exact diagnostic in the default implementation. Returning to our implementation of WatchState.c, the code now looks like:  -->
利用此扩展，可以在默认实现中提供准确的诊断。回到我们的 WatchState.c 实现，代码现在看起来像：

```c
static void defaultStop(WatchStatePtr state)
{
    /* We'll get here if the stop event isn't supported in the concrete state. */
    logUnsupportedEvent("Stop event", state->name);
}
```

<!-- Extending the State Machine -->
## 扩展状态机

<!-- One of the strengths of the STATE pattern is that it encapsulates all state-specific behavior making the state machine easy to extend.  -->
STATE 模式的优势之一是它封装了所有特定于状态的行为，使状态机易于扩展。

<!-- •Adding  a  new  event. Supporting a new event implies extending the WatchState structure with a declaration of another pointer to a function. Using the mechanism described above, a new default implementation of the event is added to WatchState.c. This step protects existing code from changes; the only impact on the concrete states is on the states that intend to support the new event, which have to implement a function, of the correct signature, to handle it. -->
* *添加一个新事件*。支持新事件意味着通过声明另一个函数指针来扩展 **WatchState** 结构（ WatchState.h ）。使用上述机制，将事件的新默认实现添加到 WatchState.c。此步骤可保护现有代码免受更改；对具体状态的唯一影响是对打算支持新事件的状态的影响，这些状态必须执行具有正确签名的函数来处理它（译注：即正确传递新状态/事件的函数指针的入参，因为函数指针类型是自定义的，不必像转换表方法必须有相同的入参类型）。

<!-- •Adding a new state. The new, concrete state has to implement functions for all events supported in that state. The only  existing  code  that  needs  to  be  changed  is  the  state  in  which  we'll  have  a  transition  to  the  new  state.  Please  note that the STATE pattern preserves one of the benefits of the table-based solution: client code, i.e. the context, remains unchanged. -->

* *添加新状态*。新的具体状态必须为该状态支持的所有事件实现函数。唯一需要更改的现有代码是状态，我们将在其中转换到新状态。请注意，STATE 模式保留了基于表格的解决方案的一个好处：客户端代码，即上下文，保持不变。

<!-- Stateless States -->
## 无状态状态

<!-- The  states  in  the  sample  code  are  stateless,  i.e.  the  WatchState  structure  only  contains  pointers  to  re-entrant  functions.  Indeed,  this  is  a  special  case  of  the  STATE  pattern  described  as  “If State objects have no instance variables [...] then contexts  can  share  a  State  object”  [2].  However,  before  sharing  any  states,  I  would  like  to  point  to  Joshua  Kerievsky's  advice  that  “it's  always  best  to  add  state-sharing  code  after  your  users  experience  system  delays  and  a  profiler  points you to the state-instantiation code as a prime bottleneck” [3].  -->
示例代码中的状态是无状态的，即 **WatchState** 结构仅包含指向可重入函数的指针。事实上，这是状态模式的一个特例，描述为“*如果状态对象没有实例变量 [...] 那么上下文可以共享一个状态对象*”[2]。但是，在共享任何状态之前，我想指出 Joshua Kerievsky 的建议：“*在您的用户遇到系统延迟并且分析器将您指出状态实例化代码作为主要瓶颈之后，最好添加状态共享代码*”[3]。

<!-- In the C language, states may be shared by declaring a static variable representing a certain state inside each function used as entry point upon a state transition. As the variables now have permanent storage, the signature of the transition functions is changed to return a pointer to the variable representing the particular state. -->
在 C 语言中，可以通过声明一个静态变量来共享状态，该变量代表每个函数内的某个状态，用作状态转换时的入口点。由于变量现在具有永久存储，因此转换函数的签名被更改以返回指向表示特定状态的变量的指针。

<!-- Listing 8: Stateless entry function, StartedState.c -->

### __清单 8：无状态入口函数，StartedState.c__

```c
WatchStatePtr transitionToStarted(void)
{
    static struct WatchState startedState;
    static int initialized = 0;

    if (0 == initialized) {
        defaultImplementation(&startedState);
        startedState.stop = stopWatch;
        initialized = 1;
    }
    return &startedState;
}
```

<!-- The  client  code  has  to  be  changed  from  holding  a  variable  representing  the  state  to  holding  a  pointer  to  the  variable  representing  the  shared  state.  Further,  the  context  has  to  define  a  callback  function  to  be  invoked  as  the  concrete  states  request a state transition.  -->
客户端代码必须从保存表示状态的变量更改为保存指向表示共享状态的变量的指针。此外，上下文必须定义要在具体状态请求状态转换时调用的回调函数。

<!-- Listing 9: Client code for changing state -->

### __清单 9：用于更改状态的客户端代码__

```c
void changeState(DigitalStopWatchPtr instance, WatchStatePtr newState) {
    /* Provides a good place for controls and trace messages (all state transitions have to go through this function). */
    instance->state = newState;
}
```

<!-- The stateless state version comes closer to the State described in Design Patterns [2] as a state transition, in contrast with the previous approach, implies changing the object pointed to by the context instead of just swapping its behaviour.  -->
与之前的方法相比，无状态状态版本更接近于设计模式 [2] 中描述的状态，因为状态转换意味着更改上下文指向的对象，而不仅仅是交换其行为。

<!-- Listing 10: State transition in StoppedState.c -->
### __清单 10：StoppedState.c 中的状态转换__

```c
static void startWatch(DigitalStopWatchPtr context)
{
    changeState(context, transitionToStarted());
}
```

<!-- A good quality of the stateless approach is that the point of state transitions is now centralized in the context. One obvious drawback is the need to pass around a reference to the context. This reference functions as a memory allowing the new state to  be  mapped  to  the  correct  context.  Another  drawback  is  the  care  that  has  to  be  taken  with  the  initialization  of  the  static  variables if the states are going to live in a multithreaded world. -->
无状态方法的一个优点是状态转换点现在集中在上下文中。一个明显的缺点是需要传递对上下文的引用。该参考用作存储器，允许将新状态映射到正确的上下文。另一个缺点是，如果状态将存在于多线程世界中，则必须注意静态变量的初始化。

<!-- Consequences -->
## __影响__

<!-- The main consequences of applying the STATE pattern are: -->
应用 STATE 模式的主要结果是：

<!-- 1.Reduces  duplication  introduced  by  complex,  state-altering  conditional  logic.  As  illustrated  in  the  example  above,  solutions  based  upon  large  segments  of  conditional  logic  tends  to  contain  duplicated  code.  The  STATE  pattern  provides an appealing alternative by removing the duplication and reducing the complexity.
2.A clear expression of the intent. The context delegates all state dependent operations to the state interface. Similar to the table-based solution, the STATE pattern lets the code reflect the intent of abstracting the problem as a finite state machine. With complex, conditional logic, that intent is typically less explicit.
3.Encapsulates the behavior of each state. Each concrete state provides a good overview of its behavior including all events  supported  in  that  very  state.  This  encapsulation  makes  it  easy  both  to  identify  as  well  as  updating  the  relevant code when changes to a certain state are to be done.
4.Implicit error handling. The solutions based on conditional logic, as well as the table-based one, requires explicit code  to  ensure  that  a  given  combination  of  state  and  event  is  valid.  Using  the  technique  described  above  of  initializing with a default implementation, the controls are built into the solution.
5.Increases the number of compilation units. The code typically becomes less compact with the STATE pattern. As Design Patterns says “such distribution is actually good if there are many states” [2].  However, for a trivial state machine  with  few,  simple  states,  the  STATE  pattern  may  introduce  an  unnecessary  complexity.  In  that  case,  if  it  isn't known that more complex behavior will be added, it is probably better to rely on conditional logic in case the logic will be easy to follow. -->

1. _减少由复杂的状态改变条件逻辑引入的重复_。如上例所示，基于大段条件逻辑的解决方案往往包含重复代码。 STATE 模式通过消除重复和降低复杂性提供了一个有吸引力的替代方案。

2. _意图的明确表达_。上下文将所有依赖于状态的操作委托给状态接口。与基于表的解决方案类似，STATE 模式让代码反映将问题抽象为有限状态机的意图。对于复杂的条件逻辑，该意图通常不那么明确。

3. _封装了每个状态的行为_。每个具体状态都提供了其行为的良好概述，包括在该状态下支持的所有事件。当要更改某个状态时，这种封装使得识别和更新相关代码变得容易。

4. _隐式错误处理_。基于条件逻辑的解决方案以及基于表的解决方案需要显式代码来确保给定的状态和事件组合是有效的。使用上述使用默认实现进行初始化的技术，控件被内置到解决方案中。

5. _增加编译单元的数量_。使用 STATE 模式，代码通常变得不那么紧凑。正如设计模式所说，“如果有很多状态，这种分布实际上是好的”[2]。然而，对于具有很少、简单状态的平凡状态机，STATE 模式可能会引入不必要的复杂性。如果不会添加更复杂的行为，逻辑易于遵循，最好依赖条件逻辑。

<!-- Summary  -->
## 总结

<!-- The STATE pattern lets us express a finite state machine, making the intent of the code clear. The behavior is partitioned on a per-state-basis and all state transitions are explicit.  -->
STATE 模式让我们表达了一个有限状态机，使代码的意图变得清晰。行为基于每个状态进行划分，并且所有状态转换都是显式的。

<!-- The STATE pattern may serve as a valuable tool when implementing complex state-dependent behavior. On the other hand, for simple problems with few states, conditional logic is probably just right.  -->
在实现复杂的状态相关行为时，STATE 模式可以作为一个有价值的工具。另一方面，对于状态很少的简单问题，条件逻辑可能恰到好处。

<!-- Next Time -->
## 接下来

<!-- We'll  continue  with  Design  Patterns  [2]  and  investigate  the  Strategy  pattern,  which  is  closely  related  to  STATE.  The  STRATEGY pattern lets us implement different variation points of an algorithm, interchangeable at run time. -->
我们将继续使用设计模式 [2] 并研究与 STATE 密切相关的 STRATEGY 模式。 STRATEGY 模式让我们可以实现算法的不同变化点，在运行时可以互换。

<!-- References  -->
## 参考

1.Adam Petersen , “Patterns in C, part 1”, C Vu 17.1
2.Gamma, E., Helm, R., Johnson, R., and Vlissides, J, “Design Patterns”, Addison-Wesley
3.Joshua Kerievsky , “Refactoring to Patterns”, Addison-Wesley

<!-- Acknowledgements  -->
致谢

Many thanks to Magnus Adamsson, Tord Andersson, and André Saitzkoff for their feedback.
