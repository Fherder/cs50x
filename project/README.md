# Currency converter addon.
#### Video demo: https://www.youtube.com/watch?v=WG8sMqa2UHw
#### Description:

This project was developed as the final project for CS50. It is a currency converter addon for Firefox. The main objective of the program is to allow users to convert currencies without needing to leave the current page or perform manual calculations.

This program uses the ExchangeRate-API, which allows the retrieval of current exchange rates for most world currencies. It also enables direct currency conversions.

#### Project files:
- **manifest.json**.
- **popup.html** .
- **styles.css**.
- **popup.js**.
- **chosen.jquery.min.js and jquery-3.6.0.min.js:** These are the scripts required for Chosen to work. Initially, I tried using external links to the JavaScript files, but Firefox has strict security rules that prevent addons from loading external scripts.

#### Manifest.json
Contains the metadata of the extension. Here, you can find the name of the addon, its version, description, and other relevant details.

```
{
  "manifest_version": 3,
  "name": "Currency Converter Addon",
  "version": "1.0",
  "description": "Convert currencies between countries.",
  "permissions": ["activeTab", "storage"],
  "host_permissions": ["https://api.exchangerate-api.com/*"],
  "action": {
    "default_popup": "popup.html",
    "default_icon": {
      "16": "icon.png",
      "48": "icon.png",
      "128": "icon.png"
    }
  }
}
```

- `manifest_version`: Specifies the version of the extension manifest. Version 3 is compatible with Chrome, Edge, Firefox, Safari, Firefox for Android, and Safari on iOS.
- `name`: The name of the extension.
- `version`: This is the version string for the extension.
- `description`: The description of the addon.
- `permissions`: This key is an array of strings, and each string is a request for a permission. 
    - `activeTab`: Grants temporary additional privileges for the active tab when the user interacts with the extension. These interactions and privileges include:
        - The user clicks the extension's browser action or page action.
        - The user selects its context menu item.
        - The user activates a keyboard shortcut defined by the extension. 
        - The ability to inject JavaScript or CSS into the tab programmatically.
        - Access to the privileged parts of the tabs API for the current tab: `Tab.url`, `Tab.title`, and `Tab.faviconUrl`.

- `host_permissions`: Requests access to specific host data. In this case, it allows the program to access the ExchangeRate-API.
- `action`: Defines the button added by the extension to the browser's toolbar. This includes the popup and the extension's icon.

#### popup.html

Defines the addon’s design. I aimed to make it as simple as possible while ensuring that the dropdown elements are comfortable to use. For this, I used **Chosen**, a jQuery plugin that enhances long and unwieldy select boxes, making them much more user-friendly perfect for handling the extensive list of world currencies.

With the help of this plugin, I enabled filtering and updating of the long list of currencies in real time as the user types each letter.

```
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Currency Converter</title>
    <link rel="stylesheet" href="styles.css">

    <!-- CSS de Chosen -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/chosen/1.8.7/chosen.min.css">
    <!-- jQuery -->
    <script src="jquery-3.6.0.min.js"></script>
    <!-- JS de Chosen -->
    <script src="chosen.jquery.min.js"></script>

  </head>
  <body>
    <div>
      <h1>Currency Converter</h1>
      <label for="amount">Amount:</label>
      <input type="number" id="amount" placeholder="Enter amount" />

      <label for="currency1">From:</label>
      <select id="currency1" class="chosen-select"></select>

      <label for="currency2">To:</label>
      <select id="currency2" class="chosen-select"></select>

      <label for="currency3">Then To:</label>
      <select id="currency3" class="chosen-select"></select>

      <button id="convert">Convert</button>
      <p id="result"></p>
    </div>
    <script src="popup.js"></script>
  </body>
</html>
```

The extension consists of a numeric input where the user specifies the amount of money they want to exchange. Additionally, it includes three select elements that represent:

1. The original currency of the specified amount.
2. The target currency to which the amount will be converted.
3. A secondary target currency for further conversion of the intermediate result.

#### styles.css
Contains the styles for the elements in the HTML.

```
body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 10px;
    background-color: #f4f4f4;
}
  
h1 {
    font-size: 1.5em;
    margin-bottom: 20px;
}
  
label {
    display: block;
    margin: 5px 0;
}
  
input, select, button {
    width: 100%;
    padding: 10px;
    margin: 10px 0;
    border: 1px solid #ccc;
    border-radius: 5px;
}
  
button {
    background-color: #007bff;
    color: white;
    border: none;
}
  
button:hover {
    background-color: #0056b3;
}
  
#result {
    font-size: 1.2em;
    margin-top: 20px;
}
```

For the styles I asked chatGPT for help, I'm actually really bad for selecting styles.

#### popup.js
This is the backend of the addon, where I programmed all the functions

```
const apiKey = 'API_KEY'; 
const apiUrl = `https://v6.exchangerate-api.com/v6/${apiKey}/latest/`;

const currency1 = document.getElementById('currency1');
const currency2 = document.getElementById('currency2');
const currency3 = document.getElementById('currency3');
const amount = document.getElementById('amount');
const result = document.getElementById('result');
const convertButton = document.getElementById('convert');
let currencies = [];

```

In the first section of the code I defined all the variables and atributes I needed.

- `apiKey`: The API key is a unique code that allows the program to make requests to the **ExchangeRate** API.
- `apiUrl`: This variable holds the base URL for the API endpoint, which is used to fetch the latest exchange rates for all currencies. The **apiKey** is dynamically included in the URL to authenticate the requests.

- `currency1`: Represents the first **dropdown** element **(select box)** where the user selects the original currency of the amount they want to convert.

- `currency2`: Represents the second **dropdown** element **(select box)** where the user selects the target currency to which the original amount will be converted.

- `currency3`: Represents the third **dropdown** element **(select box)** where the user selects an additional currency for a second conversion of the intermediate result.

- `amount`: References the numeric **input field** where the user enters the amount of money to be converted.

- `result`: References the HTML **paragraph** element where the final conversion result will be displayed after the calculations are performed.

- `convertButton`: References the Convert **button**, which triggers the conversion process when clicked.

- `currencies`: An empty array that will store the list of available currencies fetched from the API. It is later used to populate the dropdown elements (currency1, currency2, and currency3).

```
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
```

This fetchCurrencies function retrieves the available currencies from the ExchangeRate API and updates the dropdown elements with the list of currencies.

1. `async function fetchCurrencies()`: This defines an asynchronous function that fetches currency data from the API.
2. `const response = await fetch(apiUrl + 'USD')`: It makes a request to the API to get the latest exchange rates, using 'USD' as the base currency by default.
3. `const data = await response.json()`: The response from the API is converted to JSON format for easier handling of the data.
4. `currencies = Object.keys(data.conversion_rates)`: It extracts the list of currency codes (keys) from the conversion_rates object in the API response and stores it in the currencies array.
5. `updateDropdown(currency1, currencies), updateDropdown(currency2, currencies), updateDropdown(currency3, currencies)`: These functions populate the three dropdowns (currency1, currency2, and currency3) with the available currencies.
6. `initializeChosen()`: This function initializes the Chosen jQuery plugin on the updated dropdowns, making them more user-friendly.
7. `catch (error)`: If there’s an error while fetching or processing the data, it logs the error to the console.

```
function updateDropdown(dropdown, options) {
  dropdown.innerHTML = '';
  options.forEach(currency => {
    dropdown.innerHTML += `<option value="${currency}">${currency}</option>`;
  });
  $(`#${dropdown.id}`).trigger('chosen:updated');
}
```
This is the function that uses Chosen to pick up the changes, to trigger the "chosen:updated" event on the field. Chosen will re-build itself based on the updated content.

```
function initializeChosen() {
  $('.chosen-select').chosen({
    width: '100%',      
    no_results_text: 'No results found', 
    allow_single_deselect: true         
  });
}
```
The `initializeChosen()` function initializes **Chosen** customizing the `width`, the `no_results_text` reply, and allowing only one selection of currency.


```
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
```

The `convertCurrencies()` is called when the user presses the **Convert** button. It handles the conversion of the entered amount from one currency to another using the ExchangeRate API.
1. **Variable Initialization:**
    - `from`: Stores the selected original currency from the first dropdown (`currency1`).
    - `to1`: Stores the selected first target currency from the second dropdown (`currency2`).
    - `to2`: Stores the selected second target currency from the third dropdown (`currency3`).
    - `amountValue`: Retrieves the value from the amount input field, converting it to a float.
2. **Validation:**
    -  The function checks if any of the variables (`amountValue`, `from`, `to1`, `to2`) are **empty** or **invalid**.
    - If any of the fields are empty, the function displays a message saying **"Please fill in all fields."** and **stops the execution**.

3. **First API Request:** 
    - If the inputs are valid, the program constructs the **first API URL** (`apiExchangeUrl1`) to convert the `amount` from the `from` currency to the `to1` currency.
    - The first fetch request retrieves the **conversion rate**, and the **conversion result** is stored in `intermediateValue`.
4. **Second API Request:**
    - The program then uses the `intermediateValue` (the result from the first conversion) to make a second request to convert it from `to1` to `to2`.
    - The second fetch request retrieves the final **conversion rate**, and the  **conversion result** is stored in finalValue.
5. **Displaying the Result:**
If everything is okay the program displays the original **amount**, the **intermediate conversion result**, and the **final conversion result** in the result element on the webpage. The values are formatted to two decimal places using `toFixed(2)`.
 
#### Bibliography.

- How to make an extensions: [link](https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/What_are_WebExtensions)
- ExchangeRate-API documentation: [link](https://www.exchangerate-api.com/docs/overview)
- Chosen documentation: [link](https://harvesthq.github.io/chosen/)