## ImageEditorGL
基于OpenGLES-3.0 实现的图片编辑库, 利用了移动设备的GPU能力 完成图片的编辑 渲染操作

### 项目技术栈
    kotlin 原生Android UI 展示性控件
    c++ MainView 主编辑视图
    glsl es3.0 shader 图片编辑 渲染的实现

### 功能
- 涂鸦
```HTML
<video id="video" controls="" width="230" >
      <source id="tuya_id" src="https://github.com/siwangqishiq/ImageEditorGL/blob/master/media/tuya.mp4" type="video/mp4">
</video>
```

- 马赛克
<video id="video" controls="" width="230" >
      <source id="mosaic_id" src="https://github.com/siwangqishiq/ImageEditorGL/blob/master/media/mosaic.mp4" type="video/mp4">
</video>

- 裁剪
<video id="video" controls="" width="230" >
      <source id="clip_id" src="https://github.com/siwangqishiq/ImageEditorGL/blob/master/media/clip.mp4" type="video/mp4">
</video>

### 后续计划
    1.添加文字
    2.涂鸦颜色可以选择
    3.撤销操作
    4.马赛克shader优化
    5.跨平台(ios , windows , mac)
    ...





