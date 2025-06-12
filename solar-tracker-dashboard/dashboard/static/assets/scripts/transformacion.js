function ldrToSolarRadiation(ldrValue, vRef = 5.0, adcResolution = 1024) {
  // Step 1: Convert LDR value to voltage
  const voltage = (ldrValue / adcResolution) * vRef;

  // Step 2: Convert voltage to light intensity (lux)
  // Assumes a linear relationship for simplicity; modify based on your LDR's datasheet
  const lux = voltage * 500; // Example multiplier (depends on LDR calibration)

  // Step 3: Convert lux to solar radiation (W/m²)
  const solarRadiation = lux * 0.0079;

  return solarRadiation;
}

// Example usage
const ldrValue = 512; // Example LDR reading (half-scale)
const solarRadiation = ldrToSolarRadiation(ldrValue);

console.log(`Solar Radiation: ${solarRadiation.toFixed(2)} W/m²`);
