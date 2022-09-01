# PatchMatchStereo
PatchMatchStereo，经典倾斜窗口匹配算法。原项目运行在Windows上，本项目为其Linux版本。

<table>
    <tr>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/Data/Cone/im2.png"></center></td>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/Data/Cone/im6.png"></center></td>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/doc/exp/res/cone-d.png"></center></td>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/doc/exp/res/cone-c.png"></center></td>
    </tr>
    <tr>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/Data/Reindeer/view1.png"></center></td>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/Data/Reindeer/view5.png"></center></td>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/doc/exp/res/reindeer-d.png"></center></td>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/doc/exp/res/reindeer-c.png"></center></td>
    </tr>
    <tr>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/Data/Piano/im0.png"></center></td>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/Data/Piano/im1.png"></center></td>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/doc/exp/res/piano-d.png"></center></td>
        <td ><center><img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/doc/exp/res/piano-c.png"></center></td>
    </tr>
<table>
  
# CSDN博客
如果你想了解更多代码讲解，下面的博客或许能帮到你：
<br>[【码上实战】【立体匹配系列】经典PatchMatch: （1）框架](https://ethanli.blog.csdn.net/article/details/107192399)
<br>[【码上实战】【立体匹配系列】经典PatchMatch: （2）主类](https://blog.csdn.net/rs_lys/article/details/107251788)
<br>[【码上实战】【立体匹配系列】经典PatchMatch: （3）随机初始化](https://blog.csdn.net/rs_lys/article/details/107271430)
<br>[【码上实战】【立体匹配系列】经典PatchMatch: （4）代价计算](https://ethanli.blog.csdn.net/article/details/107418804)
<br>[【码上实战】【立体匹配系列】经典PatchMatch: （5）迭代传播](https://ethanli.blog.csdn.net/article/details/107436340)
<br>[【码上实战】【立体匹配系列】经典PatchMatch: （6）后处理](https://ethanli.blog.csdn.net/article/details/107549278)

# 环境
Ubuntu 20.04

# 第三方库
OpenCV4

# 算法引导
算法框架图
<div align=center>
<img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/doc/exp/算法框架.png" width=80%>
</div>
<br/>代码框架图<br/>
<div align=center>
<img src="https://github.com/sizeofbeer/PatchMatchStereo/blob/master/doc/exp/代码框架.png" width=60%>
</div>

## 备注
<b>算法优点</b>：效果好，内存占用低
<br><b>算法缺点</b>：效率低，速度比较慢，不建议跑大图，建议跑个小图看看效果（Release模式）。如果设置为前端平行窗口（PatchMatchStereo为倾斜窗口时效果最好），则速度会更快，如下：
>pms_option.is_fource_fpw = true;

## 论文
Bleyer M, Rhemann C, Rother C. <b>PatchMatch Stereo-Stereo Matching with Slanted Support Windows</b>[C]. British Machine Vision Conference 2011. 2011.