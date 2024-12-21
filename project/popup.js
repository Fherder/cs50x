const apiKey = 'API_KEY'; 
const apiUrl = `https://v6.exchangerate-api.com/v6/${apiKey}/latest/`;

const currency1 = document.getElementById('currency1');
const currency2 = document.getElementById('currency2');
const currency3 = document.getElementById('currency3');
const amount = document.getElementById('amount');
const result = document.getElementById('result');
const convertButton = document.getElementById('convert');
let currencies = [];

// Fetch currencies
async function fetchCurrencies() {
  try {
    const response = await fetch(apiUrl + 'USD'); // USD by default
    const data = await response.json();
    currencies = Object.keys(data.conversion_rates);
    updateDropdown(currency1, currencies);
    updateDropdown(currency2, currencies);
    updateDropdown(currency3, currencies);
    initializeChosen();
  } catch (error) {
    console.error("Error fetching currencies:", error);
  }
}

function initializeChosen() {
  $('.chosen-select').chosen({
    width: '100%',      
    no_results_text: 'No results found', 
    allow_single_deselect: true         
  });
}

// Function implemented with help of chatGPT
function updateDropdown(dropdown, options) {
  dropdown.innerHTML = '';
  options.forEach(currency => {
    dropdown.innerHTML += `<option value="${currency}">${currency}</option>`;
  });
  $(`#${dropdown.id}`).trigger('chosen:updated');
}

// Convert function
async function convertCurrencies() {
  try {
    const from = currency1.value;
    const to1 = currency2.value;
    const to2 = currency3.value;
    const amountValue = parseFloat(amount.value);

    if (!amountValue || !from || !to1 || !to2) {
      result.textContent = 'Please fill in all fields.';
      return;
    }

  /* 
    To get the the exchange rate from your base code to the target currency you supplied, as well as a conversion of the amount you supplied
    https://v6.exchangerate-api.com/v6/{YOUR-API-KEY}/pair/{BASE CURRENCY}/{TARGET CURRENCY}/{AMOUNT}

    {
      "result": "success",
      "documentation": "https://www.exchangerate-api.com/docs",
      "terms_of_use": "https://www.exchangerate-api.com/terms",
      "time_last_update_unix": 1585267200,
      "time_last_update_utc": "Fri, 27 Mar 2020 00:00:00 +0000",
      "time_next_update_unix": 1585270800,
      "time_next_update_utc": "Sat, 28 Mar 2020 01:00:00 +0000",
      "base_code": "EUR",
      "target_code": "GBP",
      "conversion_rate": 0.8412,
      "conversion_result": 5.8884
    }
  
  */

    const apiExchangeUrl1 = `https://v6.exchangerate-api.com/v6/${apiKey}/pair/${from}/${to1}/${amountValue}`;
    const response1 = await fetch(apiExchangeUrl1);
    const data1 = await response1.json();

    const intermediateValue =  data1.conversion_result;

    const apiExchangeUrl2 = `https://v6.exchangerate-api.com/v6/${apiKey}/pair/${to1}/${to2}/${intermediateValue}`;
    const response2 = await fetch(apiExchangeUrl2);
    const data2 = await response2.json();
    const finalValue = data2.conversion_result;

    result.textContent = `${amountValue} ${from} → ${intermediateValue.toFixed(2)} ${to1} → ${finalValue.toFixed(2)} ${to2}`;
  } catch (error) {
    console.error("Error converting currencies:", error);
    result.textContent = 'Error during conversion.';
  }
}

// Initialize
fetchCurrencies();
convertButton.addEventListener('click', convertCurrencies);
