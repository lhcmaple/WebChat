# WebChat

## 1. WebChat介绍
    WebChat顾名思义就是一个网页的聊天服务器,使用C++语言编写,数据库采用MYSQL
## 2. WebChat架构
- 任务分派机制  
    - 一个好的分派机制要动态地保持每个线程所持有的任务数的平衡,且该平衡过程的时间复杂度不能太高,可接受 *logN* 及以下的时间复杂度,所以我采用红黑树(使用开源的C算法代码,至于multiset)作为解决方案
## 3. WebChat部署
## 4. WebChat可选组件