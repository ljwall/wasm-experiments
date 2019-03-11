var Module = {

	print: function(text) {
		if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
		console.log(text);
	},

	printErr: function(text) {
		if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
		console.error(text);
	},

	readStr: (p) => {
		const buffer = new Uint8Array(Module.buffer);

		var str = '';

		while (buffer[p] !== 0) {
			str += String.fromCharCode(buffer[p++])
		}

		return str;
	},

	__dom_js: {
		elements: [null],
		context2d: [null]
	},
};
