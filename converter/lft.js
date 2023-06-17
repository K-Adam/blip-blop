const fs = require('fs');
const glob = require('glob');
const path = require('path');
const sharp = require('sharp');
const {loadImage} = require('./imageParser.js');

const input = "input";
const output = "output/lft";

function pad(number) {
    return number.toString().padStart(3, '0');
}

if (!fs.existsSync(output)) {
    fs.mkdirSync(output, { recursive: true });
}

glob.sync(`${input}/*.lft`).forEach(async filePath => {
    const fileName = path.basename(filePath);

    const dir = `${output}/${fileName}`;
    if (!fs.existsSync(dir)) {
        fs.mkdirSync(dir, { recursive: true });
    }

    const buffer = fs.readFileSync(filePath);
    let offset = 0;

    const h = buffer.readUInt32LE(offset); offset += 4;
    const spc = buffer.readUInt32LE(offset); offset += 4;

    const items = {};
    for (let i = 1; i < 256; i++) {
        const size = buffer.readUInt32LE(offset); offset += 4;
        if (size == 0) {
            continue;
        }
        // 246, 205, 148
        const data = buffer.subarray(offset, offset + size);
        offset += size;

        const imagePath = `${fileName}/${pad(i)}.png`;

        const { width, height, imageData } = loadImage(data, true);
        await sharp(imageData, { raw: { width, height, channels: 4 } }).toFile(`${output}/${imagePath}`);

        items[i] = {width, height, path: imagePath};
    }

    fs.writeFileSync(`${output}/${fileName}.json`, JSON.stringify({h, spc, items}, null, 2));
});
