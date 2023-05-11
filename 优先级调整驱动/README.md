在 C:\Users\123\Desktop\windows_driver-main\exe\testapp.c 的 157行更改thread id为你指定的县城id

在 c:\Users\123\Desktop\windows_driver-main\sys\sioctl.c 的 445 行调整优先级  最大值 31

你的测试程序需要使用administratro权限启动

测试程序：https://github.com/wqreytuk/windows_driver/blob/main/%E4%BC%98%E5%85%88%E7%BA%A7%E8%B0%83%E6%95%B4%E9%A9%B1%E5%8A%A8/%E6%B5%8B%E8%AF%95%E7%A8%8B%E5%BA%8F%E5%AF%86%E7%A0%81%E6%98%AF2.7z


调整成功后，可以在procexp中看到对应县城的动态优先级变成了你指定的优先级值


调整优先级之后的县城会获得更多的内核时间，但是并无法直接观察到相应县城的打印速率提高
