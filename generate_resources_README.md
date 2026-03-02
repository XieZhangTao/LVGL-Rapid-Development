# LVGL 资源自动生成工具（字体 + 图片）

`emb_lvgl/generate_resources.py` 用于根据 JSON 配置自动生成 LVGL 资源文件，并维护统一声明头文件 `lv_resource.h`。

它会：

- 调用 `npx lv_font_conv` 生成字体资源（`.c` / `.bin`）
- 调用 `npx ts-node node_modules/lv_img_conv/lv_img_conv.js` 生成图片资源（`.c` / `.bin`）
- 使用缓存做增量生成（时间戳 + 配置哈希）
- 清理配置已移除的旧资源
- 自动更新 `lv_resource.h`

---

## ✨ 功能概览

### 1) 字体生成

- 支持字段：`ttf_path` 或 `font_path`
- 支持格式：`.ttf` / `.otf` / `.woff` / `.woff2` / `.pfm` / `.fon`
- 支持输出：`format = "c"` 或 `"bin"`
- 支持字符范围：`range` 或 `range_file`（`range_file` 优先）

### 2) 图片生成

- 输入一般为 PNG（脚本按路径读取，不限制扩展名）
- 支持输出：`format = "c"` 或 `"bin"`
- 支持颜色格式：`color_format`（默认 `CF_TRUE_COLOR_ALPHA`）

### 3) 增量构建

脚本仅在“可能发生变化”时重建资源：

- 输出文件不存在
- 输入文件更新（时间戳变新）
- 配置哈希变化

缓存文件：

- `.font_cache.json`
- `.image_cache.json`

### 4) 自动清理

若配置中声明了 `output_dir`，脚本会在该目录下清理不再出现在配置里的旧 `.c/.bin` 文件。

### 5) 头文件生成

生成 `lv_resource.h`，包含：

- `LV_FONT_DECLARE(...)`
- `LV_IMG_DECLARE(...)`

并且支持“局部更新”：

- 只传字体配置时，会更新字体声明并保留已有图片声明
- 只传图片配置时，会更新图片声明并保留已有字体声明

---

## 🧰 环境依赖

| 依赖 | 说明 |
|---|---|
| Python 3.6+ | 运行脚本 |
| Node.js / npm / npx | 调用转换工具 |
| `lv_font_conv` | 字体转换 |
| `lv_img_conv` + `ts-node` | 图片转换 |
| Pillow（可选） | 读取图片宽高用于统计 |

安装建议：

```bash
npm install --prefix project lv_font_conv lv_img_conv ts-node
pip install Pillow
```

---

## 🚀 使用方式

### 默认配置路径

```bash
python emb_lvgl/generate_resources.py
```

默认会尝试读取工作区根目录下：

- `font_config.json`
- `image_config.json`

### 指定配置文件

```bash
python emb_lvgl/generate_resources.py path/to/font_config.json path/to/image_config.json
```

参数识别规则（按文件名包含关系）：

- 参数包含 `font` → 作为字体配置
- 参数包含 `image` → 作为图片配置
- 可只传一种配置

---

## ⚙️ 配置格式

### 字体配置 `font_config.json`

```json
{
  "output_dir": "resource/font",
  "fonts": [
    {
      "name": "my_font_16",
      "ttf_path": "assets/MyFont.ttf",
      "size": 16,
      "bpp": 4,
      "format": "bin",
      "compress": false,
      "output_path": "resource/font/my_font_16.bin",
      "range": "0x20-0x7F",
      "range_file": "assets/chars.txt"
    }
  ]
}
```

| 字段 | 必填 | 默认值 | 说明 |
|---|---|---|---|
| `name` | 是 | - | 资源名（用于声明和缓存键） |
| `ttf_path` / `font_path` | 是 | - | 字体路径（相对配置文件目录） |
| `size` | 是 | - | 字体像素大小 |
| `output_path` | 是 | - | 输出路径（相对配置文件目录） |
| `bpp` | 否 | `4` | 位深（1/2/4/8） |
| `format` | 否 | `"bin"` | `"c"` / `"bin"` |
| `compress` | 否 | `false` | 是否压缩 |
| `range` | 否 | `"0x20-0x7F"` | 字符范围 |
| `range_file` | 否 | - | 按文本内容提取字符集合（优先于 `range`） |

> 说明：若 `output_path` 未写扩展名，字体输出会自动补为 `.c` 或 `.bin`。

### 图片配置 `image_config.json`

```json
{
  "output_dir": "resource/image",
  "images": [
    {
      "name": "img_logo",
      "input_path": "assets/logo.png",
      "output_path": "resource/image/img_logo.c",
      "color_format": "CF_TRUE_COLOR_ALPHA",
      "format": "c"
    }
  ]
}
```

| 字段 | 必填 | 默认值 | 说明 |
|---|---|---|---|
| `name` | 是 | - | 资源名（用于声明和缓存键） |
| `input_path` | 是 | - | 输入图片路径 |
| `output_path` | 是 | - | 输出路径 |
| `color_format` | 否 | `"CF_TRUE_COLOR_ALPHA"` | LVGL 颜色格式 |
| `format` | 否 | `"c"` | `"c"` / `"bin"` |

> 说明：图片输出路径不会自动补扩展名，建议在 `output_path` 中写完整文件名。

---

## 📁 输出与文件位置

### 1) 资源文件

按各项配置中的 `output_path` 生成。

### 2) 头文件 `lv_resource.h`

输出位置：配置目录的上两级（即 `base_dir.parent.parent / "lv_resource.h"`）。

### 3) 缓存文件

- 字体缓存：字体配置同目录下 `.font_cache.json`
- 图片缓存：图片配置同目录下 `.image_cache.json`

若需强制全量重建，删除对应缓存文件即可。

---

## 🖥️ 典型输出

```text
LVGL 资源自动生成工具
----------------------------------------

[字体生成]
  ✓ my_font_16
  ✗ my_font_32: 字体文件不存在

[图片生成]
  ✓ img_logo
  🗑️ 删除旧文件: img_old.c

[头文件] 更新 lv_resource.h
  ✓ 新增字体: my_font_16
  ✓ 新增图片: img_logo
----------------------------------------
完成
```

---

## 🔄 处理流程

1. 读取配置文件（字体 / 图片）
2. 检查缓存与输入时间戳（决定是否跳过）
3. 生成字体、图片资源
4. 清理已移除配置对应的旧文件
5. 生成 / 更新 `lv_resource.h`
6. 保存缓存

---

## ❓常见问题

### Q1：提示“未找到任何配置文件”

请确认：

- 默认模式下工作区根目录存在 `font_config.json` 或 `image_config.json`
- 或命令行传入的配置路径文件名包含 `font` / `image`

### Q2：图片转换失败（找不到 `lv_img_conv.js`）

脚本会优先从工作区根目录查找：

`node_modules/lv_img_conv/lv_img_conv.js`

请先执行：

```bash
npm install --prefix project lv_img_conv ts-node
```

### Q3：如何让所有资源强制重建？

删除缓存文件：

- `.font_cache.json`
- `.image_cache.json`

然后重新执行脚本。
