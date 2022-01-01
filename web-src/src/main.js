import { createApp } from 'vue'
import store from './store'
import { router } from './router'
import VueProgressBar from '@aacassandra/vue3-progressbar'
import { ObserveVisibility } from 'vue-observe-visibility'
import VueClickAway from "vue3-click-away";
import App from './App.vue'

import './mystyles.scss'
import 'mdi/css/materialdesignicons.css'

createApp(App)
  .use(store)
  .use(router)
  .use(VueProgressBar, {
    color: 'hsl(204, 86%, 53%)',
    failedColor: 'red',
    height: '1px'
  })
  .use(VueClickAway)
  .directive('observe-visibility', {
    beforeMount: ObserveVisibility.beforeMount,
    update: ObserveVisibility.update,
    unmounted: ObserveVisibility.unbind
  })
  .mount('#app')
