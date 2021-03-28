import Vue from 'vue'
import app from './App.vue'

Vue.config.productionTip = false

new Vue({
  render: h => h(app),
}).$mount('#app')

require('./assets/css/main.css')