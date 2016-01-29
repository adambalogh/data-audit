install:
	node-gyp build

run:
	electron .

restart:
	npm install electron-prebuilt --save-dev
	./node_modules/.bin/electron-rebuild
	export npm_config_disturl=https://atom.io/download/atom-shell
	export npm_config_target=0.36.2
	export npm_config_arch=x64
	export npm_config_runtime=electron
	HOME=~/.electron-gyp npm install module-name
