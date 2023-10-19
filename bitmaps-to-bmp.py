from PIL import Image
from pathlib import Path
import os
import sys

def main():
    source_dir = "source_bitmaps"
    target_dir = "bitmaps"
    
    if (not os.path.exists(target_dir)):
        os.makedirs(target_dir)
    
    for source_image in Path(source_dir).glob("*.png"):
        filename = str(source_image)
        basename = source_image.stem
        transparent_image = Image.open(filename).convert("RGBA")
        
        image = Image.new("RGBA", transparent_image.size, (0x00, 0x4a, 0x7f, 0xff))
        image.paste(transparent_image, (0, 0), transparent_image)
        
        color_palette = [
            0x00, 0x4a, 0x7f, # PI_TRANSPARENT      (0)
            0x00, 0xff, 0x00, # PI_SCALES           (1)
            0xff, 0xff, 0x00, # PI_SCALES_HIGHLIGHT (2)
            0x00, 0x00, 0x00, # PI_SCALES_SHADOW    (3)
            0x00, 0x00, 0xff, # PI_HORNS            (4)
            0xff, 0x00, 0x00, # PI_EYES             (5)
            0xff, 0xff, 0xff, # PI_WHITES           (6)
            0xff, 0x00, 0xff, # PI_HORNS_SHADOW     (7)
        ]
        
        # Pillow does not let you directly specify the palette to use during a
        # quantization - it only lets you pass in an image, from which it will
        # pull the palette.
        # This forces the following dumb ass hack.
        dummy_image = Image.new("P", (1, 1), (0x00, 0x4a, 0x7f, 0xff))
        dummy_image.putpalette(color_palette)
        mapped_image = image.convert("RGB").quantize(colors=8, palette=dummy_image)
        
        mapped_image.save(os.path.join(target_dir, f"{basename}.bmp"), "BMP")
        
if __name__ == "__main__":
    main()