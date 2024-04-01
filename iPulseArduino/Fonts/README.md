# Fonts

The fonts in this folder were created with TrueType recreations of the orignal Mac fonts. The sources are listed below:

## Geneva

http://www.suppertime.co.uk/blogmywiki/2017/04/finderskeepers/

http://www.pentacom.jp/pentacom/bitfontmaker2/gallery/?id=3809

## Chicago

http://www.suppertime.co.uk/blogmywiki/2017/04/chicago/

http://www.pentacom.jp/pentacom/bitfontmaker2/gallery/?id=3780

## Monaco

https://fontstruct.com/fontstructions/show/1744750/monaco-4-4

## Espy Sans

https://thatkeith.com/fonts/espy-sans-revived/espy-sans-revived.html

## fontconvert

Additionally, the [Adafruit fontconvert utility](https://github.com/adafruit/Adafruit-GFX-Library/tree/master/fontconvert) was modified to provide more control over sizing. Change this:

```
	// << 6 because '26dot6' fixed-point format
	FT_Set_Char_Size(face, size << 6, 0, DPI, 0);
```

to:

```
	FT_Set_Char_Size(face, size, 0, 72, 0);
```

And then specify the command line size parameter using the 26.6 format. For example, to specify 12.5 pt use the integral value of 12.5 * 64.0 (800). Additionally, the output point size is set to 72 ppi, which was the [standard at the time](https://en.wikipedia.org/wiki/Macintosh_128K#Peripherals).

## Artifacts

Some of the smaller versions of Espy Sans have rendering artifacts. In theory, these could be corrected in the bitmap data for the font, but these fonts are too tiny to read at 1x scale. The 2x size does not have the artifacts.