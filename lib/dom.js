mergeInto(LibraryManager.library, {

	getElementById: function (str_ptr) {
		const id = Module.readStr(str_ptr);

		c = document.getElementById(id);

		if (c === null) return 0;

		return Module.addElement(c);
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

	context2d_setTransform: function (i, a, b, c, d, e, f) {
		Module.__dom_js.context2d[i].setTransform(a, b, c, d, e, f);
	},

	context2d_setFillStyle: function (i, str_ptr) {
		const style = Module.readStr(str_ptr);
		Module.__dom_js.context2d[i].fillStyle = style;
	},

	context2d_fillRect: function (i, x, y, width, height) {
		Module.__dom_js.context2d[i].fillRect(x, y, width, height);
	},

	context2d_lineOperations : function (i, ops_ptr, len) {
		ops_ptr = ops_ptr/4

		for (var j = 0; j < len; j++) {
			switch (Module.HEAP32[ops_ptr]) {
				case 0:
					Module.__dom_js.context2d[i].moveTo(Module.HEAPF32[ops_ptr + 1], Module.HEAPF32[ops_ptr + 2]);
					break;
				case 1:
					Module.__dom_js.context2d[i].lineTo(Module.HEAPF32[ops_ptr + 1], Module.HEAPF32[ops_ptr + 2]);
					break;
			}
			ops_ptr += 3;
		}
	},

	element_hasAttribute: function (i, str_ptr) {
		if (!(Module.__dom_js.elements[i] instanceof Element))
			throw new Error('bad element handle');

		const attr = Module.readStr(str_ptr);

		return Module.__dom_js.elements[i].hasAttribute(attr) ? 1 : 0;
	},

	now: function () {
		return Date.now();
	},

	loadImage: function (str_ptr, fp) {
		const src = Module.readStr(str_ptr),
			im = new Image(),
			h = Module.__dom_js.images.push(im) - 1;

		im.src = src;

		im.addEventListener("load", () => { Module.dynCall_vi(fp, h); });

		return h;
	},

	context2d_drawImage: function (i, h, x, y, width, height) {
		Module.__dom_js.context2d[i].drawImage(Module.__dom_js.images[h], x, y, width, height);
	},
});
