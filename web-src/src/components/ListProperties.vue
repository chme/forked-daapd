<template>
  <control-image
    v-if="item.image"
    :url="item.image"
    :caption="item.name"
    class="is-normal mb-5"
  />
  <slot v-if="$slots.buttons" name="buttons" />
  <div
    v-for="property in item.properties?.filter((p) => p.value)"
    :key="property.key"
    class="mb-3"
  >
    <div class="is-size-7 is-uppercase" v-text="$t(property.key)" />
    <div class="title is-6">
      <a
        v-if="property.handler"
        @click="property.handler"
        v-text="property.value"
      />
      <span v-else class="title is-6" v-text="property.value" />
    </div>
  </div>
</template>

<script>
import ControlImage from '@/components/ControlImage.vue'

export default {
  name: 'ListProperties',
  components: { ControlImage },
  props: {
    item: { required: true, type: Object }
  }
}
</script>
