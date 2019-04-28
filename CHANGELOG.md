<a name="3.4.6"></a>
### 3.4.6 (2019-04-28)


#### Others

*   set isScaled default value to true ([f2f6de0a](https://github.com/linuxdeepin/deepin-menu/commit/f2f6de0acd5a663183b4df29ff4d337f39723b9b))



<a name="3.4.5"></a>
### 3.4.5 (2019-04-28)


#### Features

* **menu:**  Add a description of whether to scale ([aefb41b1](https://github.com/linuxdeepin/deepin-menu/commit/aefb41b1525653b88fee1e116c0b9100c04f2fa6))



<a name="3.4.4"></a>
## 3.4.4 (2019-04-25)




<a name="3.4.3"></a>
### 3.4.3 (2019-04-12)


#### Bug Fixes

* **pos:**  HiDPI screen coordinates ([8d3cae29](https://github.com/linuxdeepin/deepin-menu/commit/8d3cae292312fc56b16e610afb8bb65641fcaefc))



<a name="3.4.2"></a>
### 3.4.2 (2019-04-12)


#### Bug Fixes

*   position shift when use kwin ([c86bbfd4](https://github.com/linuxdeepin/deepin-menu/commit/c86bbfd4643ceef903b1469ee4847b1cba770711))



<a name="3.4.1"></a>
### 3.4.1 (2018-12-07)


#### Features

*   support touchscreen ([18abfd60](https://github.com/linuxdeepin/deepin-menu/commit/18abfd60a9291c34c200ce3a8aa50ccf0cd99d39))



<a name=""></a>
##  3.4.0 (2018-09-07)


#### Bug Fixes

*   not hide menu when unregisterMenu ([cba7f135](https://github.com/linuxdeepin/deepin-menu/commit/cba7f135edbdbaa7df903f43dea86a0cfbafb038))



<a name="3.3.10"></a>
### 3.3.10 (2018-07-20)


#### Bug Fixes

*   not emit signal ([af266ccf](https://github.com/linuxdeepin/deepin-menu/commit/af266ccf7ecc615f7dc2035969694de3606ef384))



<a name="3.3.9"></a>
### 3.3.9 (2018-07-20)


#### Bug Fixes

*   wrong menu style on Qt 5.10.x ([d423e499](https://github.com/linuxdeepin/deepin-menu/commit/d423e499dbb9e4fd1a5c46a5311909776bbd6313))



<a name="3.3.8"></a>
### 3.3.8 (2018-06-13)


#### Bug Fixes

* **subment:**  submenu not send invoked singnals ([8fbf86d8](https://github.com/linuxdeepin/deepin-menu/commit/8fbf86d8b34371f10fb8af1f71ee308294633191))



<a name="3.3.7"></a>
## 3.3.7 (2018-05-31)


#### Bug Fixes

*   menu dead lock on destructor ([a865d4f9](https://github.com/linuxdeepin/deepin-menu/commit/a865d4f9040e0f0102aaa9ce68d5d8a399349b46))



<a name="3.3.6"></a>
## 3.3.6 (2018-05-30)

*   Minor bug fixes.


<a name="3.3.5"></a>
## 3.3.5 (2018-05-29)

*   Fix menu unregister but not notify signals emitted


<a name="3.3.4"></a>
## 3.3.4 (2018-05-24)


#### Features

*   Add oom score ([aeda7e58](https://github.com/linuxdeepin/deepin-menu/commit/aeda7e58658184c1305b2114fa20fd9248de2d61))

#### Bug Fixes

*   not click ([82196c3a](https://github.com/linuxdeepin/deepin-menu/commit/82196c3a4a14ff189f06831d02bc1f844b5ba53b))
*   use DRegionMonitor replace all grab ([5dc5e63f](https://github.com/linuxdeepin/deepin-menu/commit/5dc5e63f24973d17ec371ef9e10cc0387b738997))
*   click outside the menu to hide ([2a4f2f30](https://github.com/linuxdeepin/deepin-menu/commit/2a4f2f3093daadf4b751f705aa3b3583f3238a0d))



<a name="3.3.3"></a>
## 3.3.3 (2018-05-04)


#### Bug Fixes

*   desktop menu cannot hide ([80505421](https://github.com/linuxdeepin/deepin-menu/commit/80505421a26d704e9c819829651bc684412c6394))



<a name="3.3.2"></a>
### 3.3.2 (2018-03-26)


#### Bug Fixes

*   prevent memory leaks, use QPointer for MenuObject ([7511080e](https://github.com/linuxdeepin/deepin-menu/commit/7511080ec142709b7f5bdefba84627c2ad82f234))
*   crash cause menu adapter double free ([41a9b79a](https://github.com/linuxdeepin/deepin-menu/commit/41a9b79a8734cbd04b9194647420f81e52633ff3))



<a name="3.3.1"></a>
### 3.3.1 (2018-03-22)


#### Bug Fixes

* **ddesktopmenu:**  remove BypassWindowManagerHint ([92779b8c](https://github.com/linuxdeepin/deepin-menu/commit/92779b8ca7228c8fa5206acc554459aa8da6737d))



<a name=""></a>
##  3.3.0 (2018-03-08)


#### Bug Fixes

*   deepin-menu isn't counted as DE component ([200f2a77](https://github.com/linuxdeepin/deepin-menu/commit/200f2a77740149c6fa5d7ca0aa513cb194098d58))
*   Adapt lintian ([83f836e2](https://github.com/linuxdeepin/deepin-menu/commit/83f836e2a762a80b4fbec20eac39a316c894329a))
* **ddesktopmenu:**  remove DReigonMonitor ([a541e01a](https://github.com/linuxdeepin/deepin-menu/commit/a541e01a1b09d60cb6c46defaec5e85096ddc3d3))
* **debian:**  remove useless depend ([9e459981](https://github.com/linuxdeepin/deepin-menu/commit/9e45998180cfa55a9c1136772621acbca851474c))
* **desktopmenu:**  add accessible name for desktopmenu ([0f220778](https://github.com/linuxdeepin/deepin-menu/commit/0f220778c806f36a66aaa705f91390b4704e26b4))
* **dockmenu:**
  *  set border alpha is 0 ([a1d3388b](https://github.com/linuxdeepin/deepin-menu/commit/a1d3388b97418f717124897ae61b3126f36f93b7))
  *  remove use DRegionMonitor ([abc53564](https://github.com/linuxdeepin/deepin-menu/commit/abc535647ef8bf00e8ba77eb838cd593e28ae761))
  *  reduce delay time ([417d90d2](https://github.com/linuxdeepin/deepin-menu/commit/417d90d2af4338abf7e0cf259b3e7dd507e78cc6))

#### Others

*   remove autostart desktop file ([09e65bd7](https://github.com/linuxdeepin/deepin-menu/commit/09e65bd7bd46c9aa6cdd1b205aa4e4f7be33ef0c))



<a name=""></a>
##  3.2.0 (2017-11-28)


#### Bug Fixes

* **dockmenu:**  multi screen coordinates are not correct ([e2d60cfe](https://github.com/linuxdeepin/deepin-menu/commit/e2d60cfe9d5591cb48665b8ea584da7f96c1c1d8))



<a name="3.1.8"></a>
### 3.1.8 (2017-11-06)


#### Features

*   font size change with system font settings ([642f4c6a](https://github.com/linuxdeepin/deepin-menu/commit/642f4c6ad8a55fb35a8bfaf0bd3fe3efc8060862))

#### Bug Fixes

*   desktop menu hide immediately after showing ([79faf768](https://github.com/linuxdeepin/deepin-menu/commit/79faf768bf20c34a93adb6c73a4d742c5310aedd))
* **dock menu:**  no spacing between items ([fd39cc14](https://github.com/linuxdeepin/deepin-menu/commit/fd39cc144b39f2fe80335fb304ed64762252ed1a))



<a name="3.1.7"></a>
### 3.1.7 (2017-10-24)


#### Others

*   setup .clog.toml ([f751c10c](https://github.com/linuxdeepin/deepin-menu/commit/f751c10c3dbed6d920a1af2b297c38809e3e7853))

#### Bug Fixes

*   support hidpi of ddesktopmenu ([56a8cfc3](https://github.com/linuxdeepin/deepin-menu/commit/56a8cfc365eb5c2d5612f158d8db6716cec41a43))

#### Features

*   support hiDPI use DRegionMonitor ([bd5c8229](https://github.com/linuxdeepin/deepin-menu/commit/bd5c8229728d4429444ed9061d6b9cfe2f604eba))
* **hidpi:**  add basic hidpi support ([77c46c63](https://github.com/linuxdeepin/deepin-menu/commit/77c46c63f55ec8f510bab4db4e0c276c6a94e7a6))



