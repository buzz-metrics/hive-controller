function on_load() {
    get_available_wifi();
}



function reboot() {
    url = "/reboot";
    console.log(url);
    alert("Device will reboot. Discard this window. If properly configured, the light will turn green and the clock will sync within 1 hour.");

    fetch(url)
        .then(response => {
            // Check if the response is successful (status code 200-299)
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            // Parse the response body as JSON
            return response.text();
        })
        .then(response => {
            // Handle the parsed data
            console.log(response);

        })
        .catch(error => {
            // Handle any errors that occurred during the fetch
            console.error('There has been a problem with your fetch operation:', error);
        });
}


function error(msg) {
    alert(`Something went wrong:\n${msg}`);
}













function update_wifi() {
    const ssid_input = document.getElementById("ssid_input");
    const ssid = ssid_input.value;
    const password = document.getElementById("password_input").value;

    if (ssid.length < 1 || ssid.length > 30) {
        console.log("SSID does not fit length constraints");
    }

    if (password.length < 1 || password.length > 30) {
        console.log("Password does not fit length constraints");
    }

    const url = `/set_wifi_credentials?ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`;
    console.log(url);

    fetch(url)
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.text();
        })
        .then(response => {
            console.log(response);

            if (response === "Success") {
                document.getElementById("wifiResponse").innerHTML = "Wifi credentials have been updated!";
            } else {
                document.getElementById("wifiResponse").innerHTML = response;
            }
        })
        .catch(error => {
            console.error('There has been a problem with your fetch operation:', error);
        });
}


function verify_wifi() {
    const ssid_input = document.getElementById("ssid_input");
    const ssid = ssid_input.value;
    let password = document.getElementById("password_input").value;

    if (ssid.length < 1 || ssid.length > 30) {
        console.log("SSID does not fit length constraints");
    }

    if (password.length < 1 || password.length > 30) {
        console.log("Password does not fit length constraints");
    }

    url = `/verify_wifi_credentials?ssid=${ssid}&password=${password}`;
    console.log(url);



    document.getElementById("wifiResponse").innerHTML = "Attempting to verify credentials...";
    fetch(url)
        .then(response => {
            // Check if the response is successful (status code 200-299)
            if (!response.ok) {

                throw new Error('Network response was not ok');
            }
            // Parse the response body as JSON
            return response.text();
        })
        .then(response => {
            // Handle the parsed data
            console.log(response);

            if (response === "Success") {
                document.getElementById("wifiResponse").innerHTML = "Successfully verified credentials!";
            } else{
                document.getElementById("wifiResponse").innerHTML = "Failed to verify credentials";
            }


        })
        .catch(error => {
            console.error('There has been a problem with your fetch operation:', error);
        });


}




function get_available_wifi() {
    const url = `/get_available_wifi`;
    const datalist = document.getElementById("ssid_list");

    datalist.innerHTML = "";

    const defaultOption = document.createElement("option");
    defaultOption.value = "Searching...";
    defaultOption.disabled = true;
    datalist.appendChild(defaultOption);

    fetch(url)
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.text();
        })
        .then(responseText => {
            datalist.innerHTML = "";

            const ssid_list = responseText.trim().split("\n").filter(ssid => ssid);

            if (ssid_list.length === 0) {
                const noNetworksOption = document.createElement("option");
                noNetworksOption.value = "No networks found";
                noNetworksOption.disabled = true;
                datalist.appendChild(noNetworksOption);
            } else {
                ssid_list.forEach(ssid => {
                    const option = document.createElement("option");
                    option.value = ssid;
                    datalist.appendChild(option);
                });
            }
        })
        .catch(error => {
            console.error('There has been a problem with your fetch operation:', error);

            datalist.innerHTML = "";
            const errorOption = document.createElement("option");
            errorOption.value = "Error loading networks";
            errorOption.disabled = true;
            datalist.appendChild(errorOption);
        });
}


document.addEventListener('DOMContentLoaded', function () {
    get_available_wifi();
});
