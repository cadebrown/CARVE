import Vue from 'vue'
import main_layout from './main_layout.vue'

Vue.config.productionTip = false

new Vue({
  render: h => h(main_layout),
}).$mount('#main_layout')


require('./assets/css/main.css')