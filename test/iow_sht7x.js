const iow_sht7x = require('../lib/binding.js');
const assert = require('assert');

const temp = iow_sht7x.readTemperature();
const humidity = iow_sht7x.readHumidity();
    
assert(temp < 50, "temperature should not be greater than 50 deg. C.");
assert(temp > 0, "temperature should not be less than 0 deg. C.");
assert(humidity < 100, "humidity should not be greater than 100.");
assert(humidity > 0, "humidity should not be less than 0.");
