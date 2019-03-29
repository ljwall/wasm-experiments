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
		var str = '';

		while (Module.HEAPU8[p] !== 0) {
			str += String.fromCharCode(Module.HEAPU8[p++])
		}

		return str;
	},

	addElement: (el) => {
		return Module.__dom_js.elements.push(el) - 1;
	},

	getElement: (h) => {
		return Module.__dom_js.elements[h];
	},

	__dom_js: {
		elements: [null],
		images: [null],
		context2d: [null]
	},
};
