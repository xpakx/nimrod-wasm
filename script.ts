window.onload = async () => {
	console.debug('Started app');
	const canvas = document.getElementById('gameCanvas') as (HTMLCanvasElement | null);
	if (!canvas) {
		console.error('No canvas elem');
		return;
	}
	const context = canvas.getContext('2d');
	if (!context) {
		console.error("No context");
		return;
	}
	canvas.width = 500;
	canvas.height = 300;


	const frame = (_timestamp: number) => {
		// TODO
		window.requestAnimationFrame(frame);
	};
	window.requestAnimationFrame((_timestamp) => {
		// TODO
		window.requestAnimationFrame(frame);
	});
}
