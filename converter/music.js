const fs = require('fs');
const path = require('path');
const ffmpeg = require('fluent-ffmpeg');

/**
 * @param {Buffer} buffer 
 * @param {string} target
 */
module.exports.save = function(buffer, target) {
    const tempDir = fs.mkdtempSync(path.join(require('os').tmpdir(), 'temp-'));
    const tempPath = path.join(tempDir, `${Date.now()}.tmp`);

    fs.writeFileSync(tempPath, buffer);

    ffmpeg()
        .input(tempPath)
        .save(target)
        .on('end', () => {
            fs.unlinkSync(tempPath)
        });
}