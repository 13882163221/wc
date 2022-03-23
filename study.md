# 路由

## 1.什么是路由

**SPA(Single-page application)** 单页面应用

即一个web项目只有一个html页面，通过动态重写页面内容来切换视图，不需要向服务器请求新页面。SPA的出现大大提高了WEB应用的交互体验。在与用户的交互过程中，不再需要重新刷新页面，页面显示也变的更加流畅。



SPA的缺点

- SPA无法记住用户的操作记录，用户使用刷新、前进还是后退时，不能确定获取自己想要浏览的视图。

- SPA虽然能切换不同的视图，异步获取不同的内容信息，但只有一个url，对SEO不友好，不方便搜索引擎进行收录。



路由就是为了解决上述问题而出现的。



路由

是用于处理SPA不同视图的导航和切换的特殊URL，有hash和history两种模式。



**Hash**模式

区别于普通url，在url中添加了#， window.location.hash可以读取该值，通过hashchange事件可以监听其变化。

例：网易云音乐 [https://music.163.com/#/my](https://music.163.com/)[/](https://music.163.com/)

\* hash模式会被google忽略，采用 #! 方式可以被google识别



**History**模式

window.history，格式与普通url没有区别，最初只有go、forward、back几个方法用来控制页面的前进、后退、刷新。H5之后引入了pushState和replaceState方法来实现路由，可以通过popstate事件监听其变化。

例：Devui组件库 https://devui.huawei.com/components/zh-cn/overview



\* base url - <base href="/"> 浏览器要用 <base href> 的值为引用 CSS、脚本和图片文件时使用的相对 URL 添加前缀。

\* history模式需要服务端的支持，由于服务端不存在实际的url，当刷新时会返回404，需要服务端设置一个默认的index.html页面，即web项目的单页面，当刷新时返回该页面才能正常浏览。

\* Angular默认的路由模式是history。

## 2.angular路由配置

![image-20220314090836934](C:\Users\pwx1091597\AppData\Roaming\Typora\typora-user-images\image-20220314090836934.png)

## 3.angular路由过程

![image-20220314091017804](C:\Users\pwx1091597\AppData\Roaming\Typora\typora-user-images\image-20220314091017804.png)



![image-20220314091042906](C:\Users\pwx1091597\AppData\Roaming\Typora\typora-user-images\image-20220314091042906.png)



![image-20220314091103120](C:\Users\pwx1091597\AppData\Roaming\Typora\typora-user-images\image-20220314091103120.png)

![image-20220314091114576](C:\Users\pwx1091597\AppData\Roaming\Typora\typora-user-images\image-20220314091114576.png)

![image-20220314091129621](C:\Users\pwx1091597\AppData\Roaming\Typora\typora-user-images\image-20220314091129621.png)

![image-20220314091141324](C:\Users\pwx1091597\AppData\Roaming\Typora\typora-user-images\image-20220314091141324.png)

![image-20220314091153509](C:\Users\pwx1091597\AppData\Roaming\Typora\typora-user-images\image-20220314091153509.png)
