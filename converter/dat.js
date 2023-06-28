const fs = require('fs');
const glob = require('glob');
const path = require('path');

const input = "input";
const output = "output";

if (!fs.existsSync(output)) {
    fs.mkdirSync(output, { recursive: true });
}

glob.sync(`${input}/*.dat`).forEach(filePath => {
    const fileName = path.basename(filePath);

    const text = fs.readFileSync(filePath, 'utf8');

    let items = {};

    const lines = text.split(/[\r\n]+/);
    lines.forEach(line => {
        [key, value] = line.split(/\^(.*)/);

        items[key] = value;
    });

    fs.writeFileSync(`${output}/${fileName}.json`, JSON.stringify({items}, null, 2));
});
