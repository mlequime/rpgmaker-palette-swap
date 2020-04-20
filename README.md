# rpgmaker-palette-swap
Simple C++ Library to swap colours of an input RPG Maker Bitmap.
Takes two input CSV-formatted color palettes to swap, one for the other.

This allows you to create simple greyscale or other images which can then be dynamically colorized, for example to create a character or item customisation option.

To use:

Install this script above Main:
```
module PaletteSwap  
  VERSION = 1.0
  DLL_NAME = 'PaletteSwap' # Optionally change this to store the DLL elsewhere
  
  module Error
    def self.raise_error(err, obj, msg)
      klass = obj.class.to_s
      stack = caller.slice(1..15)
      stack.each do |i|
        script = i.scan(/\d+/)[0]
        i.sub!(/{\d+}/, $RGSS_SCRIPTS.at(script.to_i)[1]) if script
      end
      raise err, msg % klass << "\n\n#{stack.join("\n")}"
    end
    def self.raise_bitmap(bitmap)
      raise_error(RGSSError, bitmap, "%s is a disposed or invalid bitmap!")
    end
  end
end


#==============================================================================
# ** Bitmap
#==============================================================================

class Bitmap
  @@colorize = Win32API.new(PaletteSwap::DLL_NAME, 'Colorize', ['l', 'p', 'p'], 'l').freeze
  
  # Colorize call
  def colorize!(og_palette, new_palette)
    PaletteSwap::Error::raise_bitmap(self) if self.disposed?
    @@colorize.call(object_id, palette)
    self
  end
end
```

Then call like so (example from Essentials):

```
bmp = BitmapCache.load_bitmap("example.png")
bmp.colorize!("75,75,75\n105,105,105\n135,135,135", "50,65,98\n65,96,159\n57,119,198")
```

This will swap each color in the first list, e.g. `rgb(105,105,105)`, with the corresponding item in the second list, i.e. `rgb(65,96,159)`.


