# Friday 网络库

---

## 期望功能实现

- 高并发
- 高性能
- 日志功能
- 只支持 TCP
- 完善错误处理
- 支持优雅退出
- 服务端心跳保活
- 定时器 timerfd
- 服务端有读写 BUFFER 
- 提供序列化接口 (大小端转换, JSON)


---

服务端采用 **Reactor + one loop pre thread**

