mergeInto(LibraryManager.library, {
	initalized: function () {
		const files = Array(81).fill().map((_, i) => ({
			hour: i * 3,
			uri: 'data/gfs.t00z.ieee.0p50.f'+(i*3).toString().padStart(3, '0'),
		}));

		const draw = Module.cwrap('draw', null, ['string', 'number', 'number']);

		cb_ptr = Module.addFunction((h) => {
			document.getElementById('chart').getContext('2d').drawImage(Module.getElement(h), 0, 0)
			loop();
		}, 'vi')

		let i = 0;

		const drawOne = () => {
			if (files[i].buffer) {
				document.getElementById('chart').getContext('2d').drawImage(files[i].buffer, 0, 0)
				loop();
			} else {
				files[i].buffer = document.createElement('canvas');

				files[i].buffer.width = 1440;
				files[i].buffer.height = 771;

				const h_canvas = Module.addElement(files[i].buffer);

				draw(files[i].uri, h_canvas, cb_ptr);
			}
		}

		const loop = () => {
			i = (++i === files.length) ? 0 : i;
			setTimeout(drawOne, 200);
		};

		drawOne();
	},
});

