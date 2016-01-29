install:
	node-gyp build

run:
	electron .

restart:
	HOME=~/.electron-gyp node-gyp rebuild --target=0.36.6 --arch=x64 --dist-url=https://atom.io/download/atom-shell
