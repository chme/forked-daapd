# Vue 3 + Vite Migration

- Vue Dev Tools required in version 6 (currently only released as beta versions): <https://devtools.vuejs.org/guide/installation.html#beta>

- `vue-range-slider` is not compatible with vue3
  - replacement option: <https://github.com/vueform/slider>
- vue-router does not support navigation guards in mixins: <https://github.com/vuejs/vue-router-next/issues/454>
  - replace mixin with composition api? <https://next.router.vuejs.org/guide/advanced/composition-api.html#navigation-guards>

- vite does not support env vars in `vite.config.js` from `.env` files
  - <https://stackoverflow.com/questions/66389043/how-can-i-use-vite-env-variables-in-vite-config-js>
  - <https://github.com/vitejs/vite/issues/1930>