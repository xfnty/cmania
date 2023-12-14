# CMania: Prototype
CMania is a Osu!mania clone made for offline practice.

## Building
```
make args="./assets/test.osu"
```

## TODO
- Use `goto Failure` instead of `ASSERT_RETURN` macros.
- Rewrite or fix `sscanf` related code.
- Setup valgrind
- Improve Makefile

## References
- [`.osu` File format](https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_(file_format))
- [Raylib docs](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [ScoreV1](https://osu.ppy.sh/wiki/en/Gameplay/Score/ScoreV1/osu%21mania)
- [Judgement](https://osu.ppy.sh/wiki/en/Gameplay/Judgement/osu%21mania)
- [Osu!lazer](https://github.com/ppy/osu)
- [osu-difficulty-calculator](https://github.com/ppy/osu-difficulty-calculator)
- [McOsu](https://github.com/McKay42/McOsu)
