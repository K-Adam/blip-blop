const fs = require('fs');
const glob = require('glob');
const path = require('path');

const input = "input";
const output = "output/cin";

if (!fs.existsSync(output)) {
    fs.mkdirSync(output, { recursive: true });
}

glob.sync(`${input}/*.cin`).forEach(filePath => {
    const fileName = path.basename(filePath);

    const text = fs.readFileSync(filePath, 'utf8');
    const lines = text.split(/[\r\n]+/);

    const blocks = [];

    lines.forEach(line => {
        if (["", ";"].includes(line)) {
            return;
        }

        if (line[0] == ";") {
            const name = line.substring(2);

            blocks.push({name, items: []});
            return;
        }

        const [_, name, paramStr] = line.match(/([a-zA-Z]+)\((.*)\)/);
        const params = paramStr.trim().length ? paramStr.split(',').map(v => parseInt(v.trim())) : [];

        blocks[blocks.length-1].items.push({name, params});
    });

    fs.writeFileSync(`${output}/${fileName}.json`, JSON.stringify({blocks}, null, 2));
});
