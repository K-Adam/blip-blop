const { tab_0, tab_1, half_tone } = require('./tab.js');

/**
 * 
 * @param {Buffer} imageData 
 * @param {boolean} isTransparent
 */
module.exports.loadImage = function loadImage(imageData, isTransparent) {
   let offset = 0;

   const id = imageData.subarray(0, 3).toString(); offset += 3;
   const version = imageData.readUint8(offset); offset += 1;

   if (id != 'LGX') {
       throw new Error(`Unknown signature ${id}`);
   }

   const width = imageData.readUInt16LE(offset); offset += 2;
   const height = imageData.readUInt16LE(offset); offset += 2;
   const depth = imageData.readUInt16LE(offset); offset += 2;

   if (depth != 16) {
       throw new Error(`Non 16bit depth ${depth}`);
   }

   const trans0 = ((246 << 0) | (210 << 8) | (148 << 16)) + 0xFF000000;
   const trans1 = ((246 << 0) | (205 << 8) | (148 << 16)) + 0xFF000000;

   let target = Buffer.alloc(width * height * 4);

   if (version == 0) {
       for (let y = 0; y < height; y++) {
           for (let x = 0; x < width; x++) {
               const index = offset + (y * width + x) * 2;

               const d = imageData.readUInt16LE(index);

               if (d >= tab_0.length) {
                   throw new Error(`Out of bounds tab_0[${d}]`);
               }

               const value = tab_0[d];

               target.writeUint32LE((isTransparent && value == trans0) ? 0x0 : value, (y * width + x) * 4);
           }
       }
   } else if (version == 1) {
       let ptr = offset;
       let surfPtr = 0;
       let y = 0;
       let x = 0;
       while (y < height) {
           const d = imageData.readUInt16LE(ptr); ptr += 2;

           if (d & 0x8000) {
               const col = tab_1[imageData.readUInt16LE(ptr)]; ptr += 2;


               for (let i = 0; i < (d & 0x7FFF); i++) {
                   target.writeUint32LE((isTransparent && col == trans1) ? 0x0 : col, surfPtr);

                   surfPtr += 4;
                   x++;

                   if (x == width) {
                       x = 0;
                       y++;
                   }
               }
           } else {
               target.writeUint32LE((isTransparent && tab_1[d] == trans1) ? 0x0 : tab_1[d], surfPtr);

               surfPtr += 4;
               x++;

               if (x == width) {
                   x = 0;
                   y++;
               }
           }
       }
   } else {
       throw new Error(`Unknown version ${version}`);
   }

   return { width, height, imageData: target }
}