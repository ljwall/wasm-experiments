{
  const context = document.getElementById('bounce-animation').getContext('2d'),
    start = Date.now() / 1000,
    width = 300,
    height = 25,
    radius = 10;

  function render() {
    const t = 2*(Date.now()/1000  - start);

    context.beginPath();
    context.rect(0, 0, width, height);
    context.fillStyle = '#ffffff';
    context.fill();

    context.beginPath();
    context.arc(width/2 + (width/2 - radius)*Math.sin(t), height/2, radius, 0, 2*Math.PI)
    context.fillStyle = '#ff0000';
    context.fill();

    requestAnimationFrame(render);
  }

  render();
}
