const fs = require('fs');
const glob = require('glob');
const path = require('path');
const sharp = require('sharp');
const {loadImage} = require('./imageParser.js');

const fic_names = [
    "brief1.gfx",
    "snuf1.lvl",
    "brief1_1.gfx",
    "snuf2.lvl",
    "brief2.gfx",
    "dork.lvl",
    "brief3.gfx",
    "bisous1.lvl",
    "brief3_1.gfx",
    "bisous2.lvl",
    "bisous3.lvl",
    "brief4.gfx",
    "snorkniv.lvl",
    "snorkniv2.lvl",
    "brief5.gfx",
    "lem.lvl",
    "brief6.gfx",
    "video.lvl",
    "brief6_1.gfx",
    "videoboss.lvl",
    "brief7.gfx",
    "finalboss.lvl"
];

const nonTransparent = [
    "credits.gfx", "theend.gfx", ...fic_names
];

const input = "input";
const output = "output/gfx";

if (!fs.existsSync(output)) {
    fs.mkdirSync(output, { recursive: true });
}

glob.sync(`${input}/*.gfx`).forEach(filePath => {
    const fileName = path.basename(filePath);

    const contents = parseFile(
        fs.readFileSync(filePath),
        !nonTransparent.includes(fileName)
    );

    const data = {
        items: []
    };

    const dirName = `${output}/${fileName}`;
    if (!fs.existsSync(dirName)) {
        fs.mkdirSync(dirName, { recursive: true });
    }

    contents.forEach(async ({ xSpot, ySpot, width, height, imageData }, index) => {
        const outFilePath = `${fileName}/${pad(index)}.png`;

        data.items.push({
            path: outFilePath,
            xSpot, ySpot, width, height
        });

        await sharp(imageData, { raw: { width, height, channels: 4 } }).toFile(`${output}/${outFilePath}`);
    });

    fs.writeFileSync(`${output}/${fileName}.json`, JSON.stringify(data, null, 2));

});

function pad(number) {
    return number.toString().padStart(3, '0');
}

/**
 * @param {Buffer} buffer 
 */
function parseFile(buffer, isTransparent) {
    let offset = 0;
    const count = buffer.readUInt32LE(offset);
    offset += 4;

    let result = [];

    for (let i = 0; i < count; i++) {
        const xSpot = buffer.readUInt32LE(offset); offset += 4;
        const ySpot = buffer.readUInt32LE(offset); offset += 4;
        const size = buffer.readUInt32LE(offset); offset += 4;

        const { width, height, imageData } = loadImage(
            buffer.subarray(offset, offset + size),
            isTransparent
        );

        result.push({ xSpot, ySpot, width, height, imageData });

        offset += size;
    }

    return result;
}
