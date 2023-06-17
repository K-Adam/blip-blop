const fs = require('fs');
const glob = require('glob');
const path = require('path');

const input = "input";
const output = "output/lvl";

const EVENTID_ENNEMI            =  0;
const EVENTID_ENNEMI_GENERATOR  =  1;
const EVENTID_FOND_ANIME        =  2;
const EVENTID_MIFOND            =  3;
const EVENTID_PREMIER_PLAN      =  4;
const EVENTID_LOCK              =  5;
const EVENTID_HOLD_FIRE         =  6;
const EVENTID_FORCE_SCROLL      =  7;
const EVENTID_RPG               =  8;
const EVENTID_FLAG              =  9;
const EVENTID_TEXT              = 10;
const EVENTID_SON               = 11;
const EVENTID_METEO             = 12;
const EVENTID_OVERKILL          = 13;
const EVENTID_DYNAMIC_LOAD      = 14;
const EVENTID_MUSIC             = 15;
const EVENTID_BONUS_GENERATOR   = 16;
const EVENTID_TURRET            = 17;
const EVENTID_BONUS             = 18;

if (!fs.existsSync(output)) {
    fs.mkdirSync(output, { recursive: true });
}

/**
 * 
 * @param {Buffer} buffer 
 */
function getInts(buffer) {
    let array = [];
    for(let i=0; i<buffer.byteLength; i+=4) {
        array.push(buffer.readUInt32LE(i));
    }
    return array;
}

function pack(array) {
    let i = 0;
    let result = [];
    while(i < array.length) {
        const value = array[i];
        let count = 1;

        while((i+count) < array.length && array[i+count] == value) {
            count++;
        }

        if (count > 1) {
            result.push({value, count});
        } else {
            if (result.length > 0 && (Array.isArray(result[result.length-1]))) {
                result[result.length-1].push(value);
            } else {
                result.push([value]);
            }
        }
        i+=count;
    }

    result = result.map(item => {
        if (!Array.isArray(item) && item.count < 8) {
            return Array(item.count).fill(item.value);
        } else {
            return item;
        }
    })

    return result;
}

/**
 * @param {Buffer} buffer 
 */
function bufferToString(buffer) {
    let str = '';
    
    for (let i = 0; buffer[i] !== 0 && i < buffer.byteLength; i++) {
      str += String.fromCharCode(buffer[i]);
    }
  
    return str;
  }

glob.sync(`${input}/*.lvl`).forEach(filePath => {
    const fileName = path.basename(filePath);

    const buffer = fs.readFileSync(filePath);
    let offset = 0;

    const gfx_decors = bufferToString(buffer.subarray(offset, offset+20));offset+=20;
    const gfx_niveau = bufferToString(buffer.subarray(offset, offset+20));offset+=20;
    const gfx_ennemis = bufferToString(buffer.subarray(offset, offset+20));offset+=20;
    const sbk_ennemis = bufferToString(buffer.subarray(offset, offset+20));offset+=20;
    const fichier_mbk = bufferToString(buffer.subarray(offset, offset+20));offset+=20;
    const fichier_rpg = bufferToString(buffer.subarray(offset, offset+20));offset+=20;
    const gfx_rpg     = bufferToString(buffer.subarray(offset, offset+20));offset+=20;

    const scr_level_size = buffer.readUInt32LE(offset); offset += 4;
    const level_size = scr_level_size * 640;

    const num_decor = [];
    for(let i=0; i<scr_level_size; i++) {
        const level_decor = buffer.readUInt32LE(offset); offset += 4;
        num_decor.push(level_decor);
    }

    const xstart1 = buffer.readUInt32LE(offset); offset += 4;
    const ystart1 = buffer.readUInt32LE(offset); offset += 4;
    const xstart2 = buffer.readUInt32LE(offset); offset += 4;
    const ystart2 = buffer.readUInt32LE(offset); offset += 4;

    const vic_x     = buffer.readUInt32LE(offset); offset += 4;
    const vic_flag1 = buffer.readUInt32LE(offset); offset += 4;
    const vic_val1  = buffer.readUInt32LE(offset); offset += 4;
    const vic_flag2 = buffer.readUInt32LE(offset); offset += 4;
    const vic_val2  = buffer.readUInt32LE(offset); offset += 4;

    const NB_MAX_PLAT = 8;
    const y_plat = [];
    for (let i = 0; i < NB_MAX_PLAT; i++) {
        y_plat.push(
            pack(getInts(buffer.subarray(offset, offset + level_size*4)))
        );offset += level_size*4;
    }

    const level_size_8 = level_size / 8;
    const murs_opaques = [];
    for (let i = 0; i < 60; i++) {
        murs_opaques.push(
            pack(buffer.subarray(offset, offset + level_size_8))
        );offset += level_size_8;
    }

    const murs_sanglants = [];
    for (let i = 0; i < 60; i++) {
        murs_sanglants.push(
            pack(buffer.subarray(offset, offset + level_size_8))
        );offset += level_size_8;
    }

    const events = [];
    const nb_events = buffer.readUInt32LE(offset); offset += 4;
    for(let i=0; i<nb_events; i++) {
        const eventData = buffer.subarray(offset, offset + 60);offset += 60;

        events.push(parseEvent(eventData))
    }

    const data = {
        gfx_decors,
        gfx_niveau,
        gfx_ennemis,
        sbk_ennemis,
        fichier_mbk,
        fichier_rpg,
        gfx_rpg,

        num_decor,

        xstart1,
        ystart1,
        xstart2,
        ystart2,
        vic_x,
        vic_flag1,
        vic_val1,
        vic_flag2,
        vic_val2,

        y_plat,
        murs_opaques,
        murs_sanglants,

        events
    };

    fs.writeFileSync(`${output}/${fileName}.json`, JSON.stringify(data, null, 2));
});

/**
 * @param {Buffer} buffer 
 */
function parseEvent(buffer) {
    let offset = 0;

    const event_id = buffer.readUInt32LE(offset); offset += 4;
    const x_activation = buffer.readUInt32LE(offset); offset += 4;

    const id = buffer.readUInt32LE(offset); offset += 4;
    const x = buffer.readUInt32LE(offset); offset += 4;

    const n_pbk_sbk_txt = buffer.readUInt32LE(offset); offset += 4;
    const y_cond_speed_int_play = buffer.readUInt32LE(offset); offset += 4;
    const capacite_flag_duree = buffer.readUInt32LE(offset); offset += 4;
    const val_sens_dir = buffer.readUInt32LE(offset); offset += 4;

    const periode = buffer.readUInt32LE(offset); offset += 4;
    const tmp = !!buffer.readUInt8(offset); offset += 1;

    const fic = bufferToString(buffer.subarray(offset, offset+20));

    switch(event_id) {
        case EVENTID_ENNEMI:
            return {
                event_id,
                x_activation,
                id_ennemi: id,
                x,
                y: y_cond_speed_int_play,
                sens: val_sens_dir
            };
        case EVENTID_ENNEMI_GENERATOR:
            return {
                event_id,
                x_activation,
                id_ennemi: id,
                x,
                y: y_cond_speed_int_play,
                sens: val_sens_dir,
                capacite: capacite_flag_duree,
                periode,
                tmp
            };
        case EVENTID_LOCK:
            return {
                event_id,
                x_activation,
                cond: y_cond_speed_int_play,
                flag: capacite_flag_duree,
                val: val_sens_dir
            };
        case EVENTID_FORCE_SCROLL:
            return {
                event_id,
                x_activation,
                speed: y_cond_speed_int_play
            };
        case EVENTID_FLAG:
            return {
                event_id,
                x_activation,
                flag: capacite_flag_duree,
                val: val_sens_dir
            };
        case EVENTID_HOLD_FIRE:
            return {
                event_id,
                x_activation,
                flag: capacite_flag_duree,
                val: val_sens_dir
            };
        case EVENTID_BONUS_GENERATOR:
            return {
                event_id,
                x_activation,
                type: id,
                periode
            };
        case EVENTID_TEXT:
            return {
                event_id,
                x_activation,
                ntxt: n_pbk_sbk_txt,
                cond: y_cond_speed_int_play,
                flag: capacite_flag_duree,
                val: val_sens_dir
            };
        case EVENTID_FOND_ANIME:
            return {
                event_id,
                x_activation,
                id_fond: id,
                x,
                y: y_cond_speed_int_play,
            };
        case EVENTID_MIFOND:
            return {
                event_id,
                x_activation,
                id,
                x,
                y: y_cond_speed_int_play,
            };
        case EVENTID_PREMIER_PLAN:
            return {
                event_id,
                id_fond: id,
                x_activation,
                x,
                y: y_cond_speed_int_play,
            };
        case EVENTID_RPG:
            return {
                event_id,
                x_activation,
                num: id,
                cond: y_cond_speed_int_play,
                flag: capacite_flag_duree,
                val: val_sens_dir
            };
        case EVENTID_MUSIC:
            return {
                event_id,
                x_activation,
                id,
                play: y_cond_speed_int_play
            };
        case EVENTID_METEO:
            return {
                event_id,
                x_activation,
                intensite: y_cond_speed_int_play,
                type: id
            };
        case EVENTID_BONUS:
            return {
                event_id,
                x_activation,
                type: id,
                x,
                y: y_cond_speed_int_play,
            };
        case EVENTID_TURRET:
            return {
                event_id,
                x_activation,
                id_vehicule: id,
                x,
                y: y_cond_speed_int_play,
                dir: val_sens_dir
            };
        case EVENTID_SON:
            return {
                event_id,
                x_activation,
                nsnd: id
            };
        default:
            throw new Error(`Unknown event ${event_id}`);
    }
}
