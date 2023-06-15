const fs = require('fs');
const glob = require('glob');
const path = require('path');
const saveMp3 = require('./mp3.js').save;

const input = "input";
const output = "output/mbk";

if (!fs.existsSync(output)) {
    fs.mkdirSync(output, { recursive: true });
}

glob.sync(`${input}/*.mbk`).forEach(filePath => {
    const fileName = path.basename(filePath);

    const text = fs.readFileSync(filePath, 'utf8');
    const lines = text.split(/[\r\n]+/);

    const count = parseInt(lines.shift());

    const items = lines
        .filter(line => line.length)
        .map(line => {
            const words = line.split(/\s+/);

            const type = parseInt(words[0]);
            const musicPath = words[1].replace(/^data[\/\\]/, `${input}/`);
            if (type != 1) {
                throw new Error(`Non mp3 music in ${fileName}: ${musicPath}`);
            }

            const newName = path.basename(musicPath).replace('.zik', '.mp3')
            const newPath = `${output}/${newName}`;

            saveMp3(fs.readFileSync(musicPath), newPath);

            return newName;
        });
    
    const data = {items};
    fs.writeFileSync(`${output}/${fileName}.json`, JSON.stringify(data, null, 2));
});
