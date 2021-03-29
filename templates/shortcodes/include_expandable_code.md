<details class="box">
  <summary class="has-text-weight-bold">
   {% if summary -%}
   {{ summary }}
   {% else -%}
   Click to expand the response
   {% endif -%}
  </summary>

{% set data = load_data(path=file, format="plain") %}
```{% if language %}{{language}}{% endif %}
{{ data }}
```

</details>
