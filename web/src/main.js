import Vue from 'vue'
import App from './App.vue'
import './assets/css/style.css'

Vue.config.productionTip = false

let baseUrl = 'http://localhost:5000';
const configElement = document.getElementById( 'config' );
if (configElement) {
  const config = JSON.parse( configElement.innerHTML );
  baseUrl = config.baseUrl;
}

new Vue({
  render: h => h(App, {props: { baseUrl }})
}).$mount('#app')