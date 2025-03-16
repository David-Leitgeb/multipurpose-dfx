async function loadExamples() {
    // JSON file load
    const response = await fetch('examples.json');
    const examples = await response.json();

    examples.forEach(example => {
        const categoryDiv = document.getElementById(example.category);

        if (categoryDiv) {
            const exampleHTML = `
                <div class="example-container">
                    <!--Example title-->
                    <div class="row">
                        <h4>${example.title}</h4>
                    </div>
                    <!--Parameters-->
                    <div class="row" style="margin-top: 2px; margin-bottom: 2px;">
                        
                        <p style="margin-top: 2px; margin-bottom: 2px;">parameters: ${example.parameter_list}</p>
                    </div>
                    <!--Audio players and preset download-->
                    <div class="row">
                        <!--Input audio-->
                        <div class="audio-section">
                            <p class="audio-title input">Input</p>
                            <audio controls>
                                <source src="${example.audio_path}${example.audio_input}" type="audio/wav">
                            </audio>
                        </div>
                        <!--Output audio-->
                        <div class="audio-section">
                            <p class="audio-title output">Output</p>
                            <audio controls>
                                <source src="${example.audio_path}${example.audio_output}" type="audio/wav">
                            </audio>
                        </div>
                        <!--Preset download-->
                        <a href="${example.preset_path}${example.preset_name}" class="button" download="${example.preset_name}" style="margin-top: 39px;">
                            <img src="icons/download-simple.svg" alt="Preset" style="width: 20px; margin-right: 8px;">
                            Preset
                        </a>
                    </div>
                    <!--Figure-->
                    <div class="row">
                        <div class="image-container" style="width: ${example.figure_width}%">
                            <img src="${example.figure_path}${example.figure_name}" class="scaled-image">
                        </div>
                    </div>
                </div>
            `;

            categoryDiv.innerHTML += exampleHTML;
        }
    });
}

window.onload = loadExamples;