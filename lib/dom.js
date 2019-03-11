mergeInto(LibraryManager.library, {

	getElementById: function (str_ptr) {
		const id = Module.readStr(str_ptr);

		c = document.getElementById(id);

		if (c === null) return 0;

		return Module.__dom_js.elements.push(c) - 1;
	},

	canvas_getContext2d: function (i) {
		if (!(Module.__dom_js.elements[i] instanceof HTMLCanvasElement))
			throw new Error('bad element handle');

		const context = Module.__dom_js.elements[i].getContext('2d');

		return Module.__dom_js.context2d.push(context) - 1;
	},

	context2d_beginPath: function (i) {
		Module.__dom_js.context2d[i].beginPath();
	},

	context2d_setStrokeStyle: function (i, str_ptr) {
		const style = Module.readStr(str_ptr);
		Module.__dom_js.context2d[i].strokeStyle = style;
	},

	context2d_setLineWidth: function (i, w) {
		Module.__dom_js.context2d[i].lineWidth = w;
	},

	context2d_moveTo: function (i, x, y) {
		Module.__dom_js.context2d[i].moveTo(x, y);
	},

	context2d_lineTo: function (i, x, y) {
		Module.__dom_js.context2d[i].lineTo(x, y);
	},

	context2d_stroke: function (i) {
		Module.__dom_js.context2d[i].stroke();
	},

	element_hasAttribute: function (i, str_ptr) {
		if (!(Module.__dom_js.elements[i] instanceof Element))
			throw new Error('bad element handle');

		const attr = Module.readStr(str_ptr);

		return Module.__dom_js.elements[i].hasAttribute(attr) ? 1 : 0;
	},
});
