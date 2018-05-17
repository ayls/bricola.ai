import Vue from 'vue'
import App from './App.vue'
import makeApiClient from './services/ApiClient.js';
import makeStore from './services/VuexStore.js';
import './assets/css/style.css'

Vue.config.productionTip = false

let baseUrl = 'http://localhost:5000';
const configElement = document.getElementById( 'config' );
if (configElement) {
  const config = JSON.parse( configElement.innerHTML );
  baseUrl = config.baseUrl;
}

const apiClient = makeApiClient(baseUrl);
const store = makeStore();

new Vue({
  el: '#app',
  apiClient,
  store,
  render: h => h(App, {props: { baseUrl }})
}).$mount('#app')