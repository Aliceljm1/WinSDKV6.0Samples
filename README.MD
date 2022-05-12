# Windows sdk v6.0 sample code
 It is not easy to downloading the older version of the Windows SDK 
 Here is a link http://research.omicsgroup.org/index.php/Microsoft_Windows_SDK
 
# The directshow example doc is here:
https://docs.microsoft.com/en-us/windows/win32/directshow/directshow-samples

播放器demo，默认只支持avi格式，mp4不支持。

# Texture3D9项目：
默认实现了在一个圆柱形上渲染avi视频功能，使用了纹理进行渲染, 支持拉伸窗口，视频同步拉伸，但是马赛克很重
创建顶点缓存：			g_pd3dDevice->CreateVertexBuffer
创建纹理：				g_pd3dDevice->CreateTexture
绘制bitmap到纹理:		HRESULT CTextureRenderer::DoRenderSample( IMediaSample * pSample )
渲染窗口：				Render()
翻转圆柱体，设置几个矩阵：			SetupMatrices()

# VMRPlayer项目：
播放avi视频，支持使用VMR技术添加水印混合其他视频，支持控制位置和缩放

	控制视频缩放和位置的函数：
	TransDlgProc()

# D3D 性能优化建议
http://t.zoukankan.com/waterdragon-p-2384535.html