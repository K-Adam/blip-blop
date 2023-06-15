const fs = require('fs');
const glob = require('glob');
const path = require('path');
const ffmpeg = require('fluent-ffmpeg');

const input = "input";
const output = "output/sfx";

if (!fs.existsSync(output)) {
    fs.mkdirSync(output, { recursive: true });
}

function pad(number) {
    return number.toString().padStart(2, '0');
}

glob.sync(`${input}/*.sfx`).forEach(filePath => {
    const fileName = path.basename(filePath);

    const outputDir = `${output}/${fileName}`;
    if (!fs.existsSync(outputDir)) {
        fs.mkdirSync(outputDir, { recursive: true });
    }

    const buffer = fs.readFileSync(filePath);
    let offset = 0;

    const items = [];
    const count = buffer.readUInt32LE(offset); offset += 4;
    for(let i=0; i<count; i++) {
        const n_buff = buffer.readUInt32LE(offset); offset += 4;
        const size = buffer.readUInt32LE(offset); offset += 4;

        const soundData = buffer.subarray(offset, offset+size);
        offset += size;

        const wavPath = `${outputDir}/${i}.wav`;
        const mp3Name = `${i}.mp3`;

        items.push(mp3Name);

        fs.writeFileSync(wavPath, soundData);
        ffmpeg()
            .input(wavPath)
            .save(`${outputDir}/${mp3Name}`)
            .on('end', () => {
                fs.unlinkSync(wavPath)
            })
            ;
    }

    fs.writeFileSync(`${output}/${fileName}.json`, JSON.stringify({items}, null, 2));

});
