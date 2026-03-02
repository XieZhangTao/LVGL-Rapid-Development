#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
LVGL资源自动生成工具
通过读取resource_config.json配置文件，自动生成LVGL所需的字体和图片C文件
"""

import json
import sys
import subprocess
import platform
import hashlib
from pathlib import Path
from datetime import datetime

class ResourceGenerator:
    def __init__(self, font_config_path=None, image_config_path=None):
        self.font_config_path = Path(font_config_path) if font_config_path else None
        self.image_config_path = Path(image_config_path) if image_config_path else None
        self.font_base_dir = self.font_config_path.parent if self.font_config_path else None
        self.image_base_dir = self.image_config_path.parent if self.image_config_path else None
        self.font_config = None
        self.image_config = None
        self._use_shell = platform.system() == 'Windows'
        self.font_cache = {}
        self.image_cache = {}
        
    def _load_cache(self, cache_path):
        """加载缓存文件"""
        if cache_path.exists():
            try:
                with open(cache_path, 'r', encoding='utf-8') as f:
                    return json.load(f)
            except Exception:
                pass
        return {}
    
    def _save_cache(self, cache_path, cache_data):
        """保存缓存文件"""
        try:
            with open(cache_path, 'w', encoding='utf-8') as f:
                json.dump(cache_data, f, indent=2, ensure_ascii=False)
        except Exception:
            pass
    
    def _get_config_hash(self, config_dict):
        """计算配置字典的哈希值"""
        config_str = json.dumps(config_dict, sort_keys=True)
        return hashlib.md5(config_str.encode()).hexdigest()
        
    def load_config(self):
        """加载JSON配置文件"""
        success = True
        
        if self.font_config_path and self.font_config_path.exists():
            try:
                with open(self.font_config_path, 'r', encoding='utf-8') as f:
                    self.font_config = json.load(f)
            except Exception as e:
                print(f"✗ 字体配置加载失败: {e}")
                success = False
        
        if self.image_config_path and self.image_config_path.exists():
            try:
                with open(self.image_config_path, 'r', encoding='utf-8') as f:
                    self.image_config = json.load(f)
            except Exception as e:
                print(f"✗ 图片配置加载失败: {e}")
                success = False
        
        if not self.font_config and not self.image_config:
            print("✗ 未找到任何配置文件")
            return False
            
        return success
    
    def _find_font_file(self, font_config, supported_formats):
        """查找字体文件，支持多种格式"""
        for field in ['ttf_path', 'font_path']:
            if field in font_config:
                font_path = self.font_base_dir / font_config[field]
                if font_path.exists():
                    return font_path
        
        # 自动查找同目录下的支持格式文件
        for field in ['ttf_path', 'font_path']:
            if field in font_config:
                base_path = self.font_base_dir / font_config[field]
                for ext in supported_formats:
                    candidate = base_path.parent / (base_path.stem + ext)
                    if candidate.exists():
                        return candidate
        
        return None
    
    def _clean_old_fonts(self):
        """清理配置中不存在的旧字体文件"""
        if not self.font_config or 'fonts' not in self.font_config:
            return
        
        # 收集当前配置的所有输出文件
        current_outputs = set()
        for font in self.font_config['fonts']:
            output_path = self.font_base_dir / font['output_path']
            format_type = font.get('format', 'bin')
            out_path = output_path if output_path.suffix else Path(str(output_path) + ('.c' if format_type == 'c' else '.bin'))
            current_outputs.add(out_path.resolve())
        
        # 扫描输出目录，删除不在配置中的文件
        if self.font_config.get('output_dir'):
            output_dir = self.font_base_dir / self.font_config['output_dir']
            if output_dir.exists():
                for file_path in output_dir.rglob('*'):
                    if file_path.is_file() and file_path.suffix in {'.c', '.bin'} and file_path.resolve() not in current_outputs:
                        try:
                            file_path.unlink()
                            print(f"  🗑️ 删除旧文件: {file_path.name}")
                        except Exception as e:
                            print(f"  ✗ 删除失败 {file_path.name}: {e}")
    
    def generate_fonts(self):
        """生成LVGL字体文件"""
        if not self.font_config or 'fonts' not in self.font_config or not self.font_config['fonts']:
            return
        
        print("\n[字体生成]")
        self._clean_old_fonts()
        
        # 加载缓存
        cache_path = self.font_base_dir / '.font_cache.json'
        self.font_cache = self._load_cache(cache_path)
        
        supported_formats = {'.ttf', '.otf', '.woff', '.woff2', '.pfm', '.fon'}
        font_stats = []
        
        for font in self.font_config['fonts']:
            try:
                font_path = self._find_font_file(font, supported_formats)
                if not font_path:
                    print(f"  ✗ {font.get('name', 'unknown')}: 字体文件不存在")
                    continue
                
                output_path = self.font_base_dir / font['output_path']
                output_path.parent.mkdir(parents=True, exist_ok=True)
                
                # 处理字符范围
                char_range = '0x20-0x7F'
                range_mtime = 0
                if 'range_file' in font:
                    range_file_path = self.font_base_dir / font['range_file']
                    if range_file_path.exists():
                        try:
                            with open(range_file_path, 'r', encoding='utf-8') as f:
                                text_content = f.read()
                            unique_chars = set(c for c in text_content if ord(c) >= 0x20 and c not in '\n\r\t')
                            if unique_chars:
                                char_range = ','.join(sorted([hex(ord(c)) for c in unique_chars]))
                            range_mtime = range_file_path.stat().st_mtime
                        except Exception:
                            pass
                elif 'range' in font:
                    char_range = font['range']
                
                format_type = font.get('format', 'bin')
                lvgl_format = 'lvgl' if format_type == 'c' else 'bin'
                out_path = output_path if output_path.suffix else Path(str(output_path) + ('.c' if format_type == 'c' else '.bin'))
                
                # 计算当前配置的哈希值
                config_key = {
                    'name': font['name'],
                    'size': font['size'],
                    'bpp': font.get('bpp', 4),
                    'format': format_type,
                    'compress': font.get('compress', False),
                    'range': char_range,
                    'ttf_path': str(font_path)  # 添加字体文件路径到哈希计算
                }
                current_hash = self._get_config_hash(config_key)
                cached_hash = self.font_cache.get(font['name'], {}).get('hash', '')
                
                # 增量检查：输出文件存在 且 输入文件未修改 且 配置未改变
                if (out_path.exists() and 
                    out_path.stat().st_mtime > font_path.stat().st_mtime and 
                    out_path.stat().st_mtime > range_mtime and 
                    current_hash == cached_hash):
                    font_stats.append({
                        'name': font['name'], 'size': font['size'], 'format': format_type,
                        'bpp': font.get('bpp', 4), 'file_size': out_path.stat().st_size,
                        'output_path': str(out_path.relative_to(self.font_base_dir))
                    })
                    continue
                
                cmd = [
                    'npx', 'lv_font_conv', '--font', str(font_path), '--size', str(font['size']),
                    '--format', lvgl_format, '--bpp', str(font.get('bpp', 4)), '--output', str(out_path),
                    '--range', char_range, '--compress' if font.get('compress', False) else '--no-compress'
                ]
                
                result = subprocess.run(cmd, capture_output=True, text=True, cwd=self.font_base_dir, shell=self._use_shell)
                
                if result.returncode == 0 and out_path.exists():
                    file_size = out_path.stat().st_size
                    font_stats.append({
                        'name': font['name'], 'size': font['size'], 'format': format_type,
                        'bpp': font.get('bpp', 4), 'file_size': file_size,
                        'output_path': str(out_path.relative_to(self.font_base_dir))
                    })
                    # 更新缓存
                    self.font_cache[font['name']] = {'hash': current_hash}
                    print(f"  ✓ {font['name']}")
                else:
                    print(f"  ✗ {font['name']}: {result.stderr.strip()}")
                    
            except Exception as e:
                print(f"  ✗ {font.get('name', 'unknown')}: {e}")
        
        # 保存缓存
        self._save_cache(cache_path, self.font_cache)
    
    def _clean_old_images(self):
        """清理配置中不存在的旧图片文件"""
        if not self.image_config or 'images' not in self.image_config:
            return
        
        # 收集当前配置的所有输出文件
        current_outputs = set()
        for image in self.image_config['images']:
            output_path = self.image_base_dir / image['output_path']
            current_outputs.add(output_path.resolve())
        
        # 扫描输出目录，删除不在配置中的文件
        if self.image_config.get('output_dir'):
            output_dir = self.image_base_dir / self.image_config['output_dir']
            if output_dir.exists():
                for file_path in output_dir.rglob('*'):
                    if file_path.is_file() and file_path.suffix in {'.c', '.bin'} and file_path.resolve() not in current_outputs:
                        try:
                            file_path.unlink()
                            print(f"  🗑️ 删除旧文件: {file_path.name}")
                        except Exception as e:
                            print(f"  ✗ 删除失败 {file_path.name}: {e}")
    
    def generate_images(self):
        """生成LVGL图片文件"""
        if not self.image_config or 'images' not in self.image_config or not self.image_config['images']:
            return
        
        print("\n[图片生成]")
        self._clean_old_images()
        
        # 加载缓存
        cache_path = self.image_base_dir / '.image_cache.json'
        self.image_cache = self._load_cache(cache_path)
        
        image_stats = []
        project_root = Path(__file__).parent.resolve()
        lv_img_conv_path = project_root / 'node_modules' / 'lv_img_conv' / 'lv_img_conv.js'
        
        for image in self.image_config['images']:
            try:
                input_path = self.image_base_dir / image['input_path']
                output_path = self.image_base_dir / image['output_path']
                
                if not input_path.exists():
                    print(f"  ✗ {image['name']}: 文件不存在")
                    continue
                
                output_path.parent.mkdir(parents=True, exist_ok=True)
                
                color_format = image.get('color_format', 'CF_TRUE_COLOR_ALPHA')
                format_type = image.get('format', 'c')
                output_format = 'c' if format_type == 'c' else 'bin'
                
                # 获取图片尺寸
                width, height = 0, 0
                try:
                    from PIL import Image as PILImage
                    with PILImage.open(input_path) as img:
                        width, height = img.size
                except Exception:
                    pass
                
                # 计算当前配置的哈希值
                config_key = {
                    'name': image['name'],
                    'color_format': color_format,
                    'format': format_type,
                    'input_path': str(input_path)  # 添加输入文件路径到哈希计算
                }
                current_hash = self._get_config_hash(config_key)
                cached_hash = self.image_cache.get(image['name'], {}).get('hash', '')
                
                # 增量检查：输出文件存在 且 输入文件未修改 且 配置未改变
                if (output_path.exists() and 
                    output_path.stat().st_mtime > input_path.stat().st_mtime and 
                    current_hash == cached_hash):
                    image_stats.append({
                        'name': image['name'], 'width': width, 'height': height,
                        'format': format_type, 'color_format': color_format,
                        'file_size': output_path.stat().st_size,
                        'output_path': str(output_path.relative_to(self.image_base_dir))
                    })
                    continue
                
                cmd = [
                    'npx', 'ts-node', str(lv_img_conv_path), str(input_path),
                    '-c', color_format, '-t', output_format, '-o', str(output_path), '-f'
                ]
                
                result = subprocess.run(cmd, capture_output=True, text=True, cwd=project_root, shell=self._use_shell)
                
                if result.returncode == 0 and output_path.exists():
                    file_size = output_path.stat().st_size
                    image_stats.append({
                        'name': image['name'], 'width': width, 'height': height,
                        'format': format_type, 'color_format': color_format,
                        'file_size': file_size, 'output_path': str(output_path.relative_to(self.image_base_dir))
                    })
                    # 更新缓存
                    self.image_cache[image['name']] = {'hash': current_hash}
                    print(f"  ✓ {image['name']}")
                else:
                    print(f"  ✗ {image['name']}: {result.stderr.strip()}")
                    
            except Exception as e:
                print(f"  ✗ {image.get('name', 'unknown')}: {e}")
        
        # 保存缓存
        self._save_cache(cache_path, self.image_cache)
    
    def _parse_existing_header(self, header_path):
        """解析现有头文件，提取已有的字体和图片声明"""
        import re
        existing_fonts, existing_images = set(), set()
        
        if not header_path.exists():
            return existing_fonts, existing_images
        
        try:
            with open(header_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            for match in re.finditer(r'LV_FONT_DECLARE\((\w+)\)', content):
                existing_fonts.add(match.group(1))
            for match in re.finditer(r'LV_IMG_DECLARE\((\w+)\)', content):
                existing_images.add(match.group(1))
        except Exception:
            pass
        
        return existing_fonts, existing_images
    
    def generate_header_file(self):
        """生成资源声明头文件（只更新提供了配置的资源类型）"""
        # 使用第一个可用的 base_dir 来确定头文件位置
        base_dir = self.font_base_dir or self.image_base_dir
        if not base_dir:
            return
        
        header_path = base_dir.parent.parent / 'lv_resource.h'
        existing_fonts, existing_images = self._parse_existing_header(header_path)
        
        # 只更新提供了配置文件的资源类型
        if self.font_config and 'fonts' in self.font_config:
            # 提供了字体配置，更新字体列表
            all_fonts = set()
            for font in self.font_config['fonts']:
                all_fonts.add(font['name'])
        else:
            # 未提供字体配置，保留现有字体声明
            all_fonts = existing_fonts
        
        if self.image_config and 'images' in self.image_config:
            # 提供了图片配置，更新图片列表
            all_images = set()
            for image in self.image_config['images']:
                all_images.add(image['name'])
        else:
            # 未提供图片配置，保留现有图片声明
            all_images = existing_images
        
        try:
            with open(header_path, 'w', encoding='utf-8') as f:
                f.write("/**\n * @file lv_resource.h\n * @brief LVGL资源声明文件 (自动生成)\n */\n\n")
                f.write("#ifndef LV_RESOURCE_H\n#define LV_RESOURCE_H\n\n")
                f.write("#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n")
                f.write("#if defined(_WIN64)\n#   include \"lvgl/lvgl.h\"\n#else\n#   include \"lvgl.h\"\n#endif\n\n")
                
                if all_fonts:
                    f.write("/* 字体声明 */\n")
                    for name in sorted(all_fonts):
                        f.write(f"LV_FONT_DECLARE({name});\n")
                    f.write("\n")
                
                if all_images:
                    f.write("/* 图片声明 */\n")
                    for name in sorted(all_images):
                        f.write(f"LV_IMG_DECLARE({name});\n")
                    f.write("\n")
                
                f.write("#ifdef __cplusplus\n} /* extern \"C\" */\n#endif\n\n#endif /* LV_RESOURCE_H */\n")
            
            new_fonts = all_fonts - existing_fonts
            new_images = all_images - existing_images
            removed_fonts = existing_fonts - all_fonts
            removed_images = existing_images - all_images
            
            if new_fonts or new_images or removed_fonts or removed_images:
                print(f"\n[头文件] 更新 {header_path.name}")
                # 只显示提供了配置的资源类型的变化
                if self.font_config:
                    if new_fonts:
                        print(f"  ✓ 新增字体: {', '.join(sorted(new_fonts))}")
                    if removed_fonts:
                        print(f"  🗑️ 删除字体: {', '.join(sorted(removed_fonts))}")
                if self.image_config:
                    if new_images:
                        print(f"  ✓ 新增图片: {', '.join(sorted(new_images))}")
                    if removed_images:
                        print(f"  🗑️ 删除图片: {', '.join(sorted(removed_images))}")
            
        except Exception as e:
            print(f"✗ 头文件生成失败: {e}")
    

    
    def run(self):
        """执行资源生成流程"""
        print("LVGL 资源自动生成工具")
        print("-" * 40)
        
        if not self.load_config():
            return False
        
        self.generate_fonts()
        self.generate_images()
        self.generate_header_file()
        
        print("-" * 40)
        print("完成")
        return True

def main():
    font_config, image_config = None, None
    
    if len(sys.argv) > 1:
        for arg in sys.argv[1:]:
            if 'font' in arg:
                font_config = arg
            elif 'image' in arg:
                image_config = arg
    else:
        script_dir = Path(__file__).parent
        font_config = script_dir / 'font_config.json'
        image_config = script_dir / 'image_config.json'
    
    generator = ResourceGenerator(font_config, image_config)
    sys.exit(0 if generator.run() else 1)

if __name__ == '__main__':
    main()
