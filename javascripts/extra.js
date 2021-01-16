document.querySelectorAll('img.zoom').forEach(item => {
    item.addEventListener('click', function () {
        const img = document.getElementById('fullscreen-image-img');
        img.setAttribute('src', this.getAttribute('src'));
        img.setAttribute('alt', this.getAttribute('alt'));

        const div = document.getElementById('fullscreen-image');
        div.classList.replace('hidden', 'visible');
    })
});

var div = document.createElement('div');
div.classList.add('fullscreen-image-background', 'hidden');
div.id = 'fullscreen-image';
var img = document.createElement('img');
img.id = 'fullscreen-image-img';
div.appendChild(img);

div.addEventListener('click', function () {
    this.classList.replace('visible', 'hidden');
});
document.body.appendChild(div);