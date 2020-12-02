import Vue from 'vue'
import App from './App.vue'
import makeApiClient from './services/ApiClient.js';
import makeStore from './services/VuexStore.js';
import './assets/css/style.css'

Vue.config.productionTip = false

const configElement = document.getElementById( 'config' );
const config = JSON.parse( configElement.innerHTML );
const baseUrl = config.baseUrl;

const apiClient = makeApiClient(baseUrl);
const store = makeStore();

new Vue({
  el: '#app',
  apiClient,
  store,
  render: h => h(App, {props: { baseUrl }})
}).$mount('#app')