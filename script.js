let settingsOpen = false;
let infoOpen = false;

const toggleSettings = open => {
    settingsOpen = open;
    document.getElementById('settings').style.width = settingsOpen ? '100%' : '0%';
}

const toggleInfo = open => {
    infoOpen = open;
    document.getElementById('info').style.width = infoOpen ? '100%' : '0%';
}
