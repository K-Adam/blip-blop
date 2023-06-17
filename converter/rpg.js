const fs = require('fs');
const glob = require('glob');
const path = require('path');

const input = "input";
const output = "output/rpg";

if (!fs.existsSync(output)) {
    fs.mkdirSync(output, { recursive: true });
}

function parse(value) {
    if (!isNaN(value)) {
        if(value.toString().indexOf('.') != -1) {
            return parseFloat(value);
        } else {
            return parseInt(value);
        }
    } else {
        return value;
    }
}

glob.sync(`${input}/*.rpg`).forEach(filePath => {
    const fileName = path.basename(filePath);

    const text = fs.readFileSync(filePath, 'utf8');
    const lines = text.split(/[\r\n]+/);

    const blocks = [];

    lines.forEach(line => {
        if (["", ";", "stop"].includes(line)) {
            return;
        }

        if (line[0] == ";") {
            const name = line.substring(2);

            blocks.push({name, items: []});
            return;
        }

        const [key, value] = line.split('=');
        blocks[blocks.length-1].items.push([key, parse(value)]);

    });

    fs.writeFileSync(`${output}/${fileName}.json`, JSON.stringify({blocks}, null, 2));
});
