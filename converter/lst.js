const fs = require('fs');
const glob = require('glob');
const path = require('path');

const input = "input";
const output = "output";

if (!fs.existsSync(output)) {
    fs.mkdirSync(output, { recursive: true });
}

const PART_CINE     = 0;
const PART_LEVEL    = 1;
const PART_BRIEFING = 2;

glob.sync(`${input}/*.lst`).forEach(filePath => {
    const fileName = path.basename(filePath);

    const text = fs.readFileSync(filePath, 'utf8');
    const lines = text.split(/[\r\n]+/);

    const items = lines
    .filter(line => line.length)
    .map(line => {
        const words = line.split(/\s+/);
        const typeNum = parseInt(words[0]);

        if(![1,2].includes(typeNum)) {
            throw new Error(`Unknown type ${typeNum}`);
        }

        const type = typeNum == 1 ? 'LEVEL' : 'BRIEFING';

        if (typeNum == 1) {
            return {
                type,
                level: parseInt(words[1]),
                path: words[2].replace(/^\*data[\\/](.*)$/, "lvl/$1")
            };
        } else {
            return {
                type,
                path: words[1].replace(/^\*data[\\/](.*)$/, "gfx/$1")
            };
        }
    });

    const data = {items};
    fs.writeFileSync(`${output}/${fileName}.json`, JSON.stringify(data, null, 2));
});
