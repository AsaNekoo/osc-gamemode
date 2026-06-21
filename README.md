# [osc-gamemode](https://github.com/AsaNekoo/osc-gamemode)

A Geode mod for Geometry Dash that sends the player's current gamemode over OSC, intended for use with VRChat.

## Demo

<img width="480" height="230" alt="output" src="https://github.com/user-attachments/assets/157c9160-798c-4985-8731-0698ceb60c3c" />

## VRChat Asset Requirements

The companion VRChat asset requires:
- [Modular Avatar](https://modular-avatar.nadena.dev/)
- [liltoon](https://lilxyzw.github.io/lilToon/)

## Textures

All textures used by the asset should be **512x512**. You can grab base icon textures from [gdbrowser.com/iconkit](https://gdbrowser.com/iconkit/) and edit them down to fit that size.

If you are on Linux and have ImageMagick installed, you can run the following command in the directory with your icons to batch resize and format them:
```shell
mkdir -p out && magick *.png -resize 512x512 -background none -gravity center -extent 512x512 -set filename:f "%t" "out/%[filename:f].png"
```

## Building the Mod

```
# Assuming you have the Geode CLI set up already
geode build
```

## Resources

- [Geode SDK Documentation](https://docs.geode-sdk.org/)
- [Geode SDK Source Code](https://github.com/geode-sdk/geode/)
- [Icon Kit](https://www.youtube.com/watch?v=mCmBrWdTQ98)
