"""
Model Runner - Python Bridge for AI Forge Studio

This module provides a Python interface for running AI models that may not be
directly accessible from C++. Supports PyTorch, Transformers, and Diffusers.

Features:
- Text-to-Image generation (Stable Diffusion, SDXL)
- Image upscaling (Real-ESRGAN, GFPGAN)
- Text generation (LLM inference)
- Model caching and optimization
- CUDA memory management
"""

import os
import sys
import json
import numpy as np
from typing import Dict, List, Optional, Tuple, Any
from dataclasses import dataclass
from enum import Enum
import logging

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='[%(asctime)s] [%(levelname)s] [%(name)s] %(message)s'
)
logger = logging.getLogger("ModelRunner")

# Try to import AI libraries
try:
    import torch
    TORCH_AVAILABLE = True
    logger.info(f"PyTorch {torch.__version__} available")
    logger.info(f"CUDA available: {torch.cuda.is_available()}")
    if torch.cuda.is_available():
        logger.info(f"CUDA device: {torch.cuda.get_device_name(0)}")
except ImportError:
    TORCH_AVAILABLE = False
    logger.warning("PyTorch not available")

try:
    from diffusers import StableDiffusionPipeline, DPMSolverMultistepScheduler
    DIFFUSERS_AVAILABLE = True
    logger.info("Diffusers library available")
except ImportError:
    DIFFUSERS_AVAILABLE = False
    logger.warning("Diffusers not available")

try:
    from transformers import AutoTokenizer, AutoModelForCausalLM
    TRANSFORMERS_AVAILABLE = True
    logger.info("Transformers library available")
except ImportError:
    TRANSFORMERS_AVAILABLE = False
    logger.warning("Transformers not available")


class ModelType(Enum):
    """Model type enumeration"""
    TEXT_TO_IMAGE = "text_to_image"
    IMAGE_TO_IMAGE = "image_to_image"
    TEXT_GENERATION = "text_generation"
    IMAGE_UPSCALING = "image_upscaling"


@dataclass
class InferenceConfig:
    """Configuration for inference operations"""
    model_id: str
    prompt: str = ""
    negative_prompt: str = ""
    num_inference_steps: int = 50
    guidance_scale: float = 7.5
    width: int = 512
    height: int = 512
    seed: Optional[int] = None
    batch_size: int = 1
    precision: str = "fp16"  # fp32, fp16, bf16
    use_attention_slicing: bool = True
    use_cpu_offload: bool = False


@dataclass
class InferenceResult:
    """Result from inference operation"""
    success: bool
    error_message: str = ""
    image_data: Optional[np.ndarray] = None
    text_output: str = ""
    inference_time: float = 0.0
    memory_used: int = 0


class ModelCache:
    """Cache for loaded models to avoid reloading"""

    def __init__(self):
        self.models: Dict[str, Any] = {}
        self.model_types: Dict[str, ModelType] = {}

    def add(self, model_id: str, model: Any, model_type: ModelType):
        """Add model to cache"""
        self.models[model_id] = model
        self.model_types[model_id] = model_type
        logger.info(f"Cached model: {model_id}")

    def get(self, model_id: str) -> Optional[Any]:
        """Get model from cache"""
        return self.models.get(model_id)

    def has(self, model_id: str) -> bool:
        """Check if model is cached"""
        return model_id in self.models

    def remove(self, model_id: str):
        """Remove model from cache"""
        if model_id in self.models:
            del self.models[model_id]
            del self.model_types[model_id]
            logger.info(f"Removed model from cache: {model_id}")

    def clear(self):
        """Clear all cached models"""
        self.models.clear()
        self.model_types.clear()
        logger.info("Cleared model cache")


class ModelRunner:
    """Main model runner class"""

    def __init__(self, device: str = "cuda"):
        """
        Initialize model runner

        Args:
            device: Device to run on ('cuda' or 'cpu')
        """
        self.device = device
        self.cache = ModelCache()

        # Check CUDA availability
        if device == "cuda" and not torch.cuda.is_available():
            logger.warning("CUDA not available, falling back to CPU")
            self.device = "cpu"

        logger.info(f"ModelRunner initialized on device: {self.device}")

    def load_text_to_image_model(
        self,
        model_path: str,
        model_id: str,
        precision: str = "fp16"
    ) -> bool:
        """
        Load a text-to-image model (Stable Diffusion)

        Args:
            model_path: Path to model or HuggingFace model ID
            model_id: Unique identifier for this model
            precision: Precision mode (fp32, fp16)

        Returns:
            True if successful
        """
        if not TORCH_AVAILABLE or not DIFFUSERS_AVAILABLE:
            logger.error("Required libraries not available")
            return False

        try:
            logger.info(f"Loading text-to-image model: {model_path}")

            # Determine dtype
            dtype = torch.float16 if precision == "fp16" else torch.float32

            # Load pipeline
            pipe = StableDiffusionPipeline.from_pretrained(
                model_path,
                torch_dtype=dtype,
                safety_checker=None  # Disable for performance
            )

            # Optimize
            pipe.scheduler = DPMSolverMultistepScheduler.from_config(
                pipe.scheduler.config
            )
            pipe = pipe.to(self.device)

            # Enable memory optimizations
            pipe.enable_attention_slicing()
            if hasattr(pipe, 'enable_xformers_memory_efficient_attention'):
                try:
                    pipe.enable_xformers_memory_efficient_attention()
                    logger.info("Enabled xformers memory efficient attention")
                except Exception as e:
                    logger.warning(f"Could not enable xformers: {e}")

            # Cache model
            self.cache.add(model_id, pipe, ModelType.TEXT_TO_IMAGE)

            logger.info(f"Model loaded successfully: {model_id}")
            return True

        except Exception as e:
            logger.error(f"Failed to load model: {e}")
            return False

    def generate_image(self, config: InferenceConfig) -> InferenceResult:
        """
        Generate image from text prompt

        Args:
            config: Inference configuration

        Returns:
            InferenceResult with generated image
        """
        result = InferenceResult(success=False)

        if not self.cache.has(config.model_id):
            result.error_message = f"Model not loaded: {config.model_id}"
            logger.error(result.error_message)
            return result

        try:
            import time
            start_time = time.time()

            # Get model
            pipe = self.cache.get(config.model_id)

            # Set seed if specified
            generator = None
            if config.seed is not None:
                generator = torch.Generator(device=self.device).manual_seed(config.seed)

            # Get memory before inference
            if self.device == "cuda":
                torch.cuda.synchronize()
                mem_before = torch.cuda.memory_allocated() / (1024 ** 2)  # MB
            else:
                mem_before = 0

            logger.info(f"Generating image with prompt: {config.prompt[:50]}...")

            # Generate image
            output = pipe(
                prompt=config.prompt,
                negative_prompt=config.negative_prompt,
                num_inference_steps=config.num_inference_steps,
                guidance_scale=config.guidance_scale,
                width=config.width,
                height=config.height,
                generator=generator,
                num_images_per_prompt=config.batch_size
            )

            # Get memory after inference
            if self.device == "cuda":
                torch.cuda.synchronize()
                mem_after = torch.cuda.memory_allocated() / (1024 ** 2)  # MB
            else:
                mem_after = 0

            # Convert to numpy array
            image = output.images[0]
            result.image_data = np.array(image)

            # Calculate stats
            result.inference_time = (time.time() - start_time) * 1000  # ms
            result.memory_used = int(mem_after - mem_before)
            result.success = True

            logger.info(f"Image generated in {result.inference_time:.2f}ms")

        except Exception as e:
            result.error_message = str(e)
            logger.error(f"Image generation failed: {e}")

        return result

    def unload_model(self, model_id: str) -> bool:
        """
        Unload a model from memory

        Args:
            model_id: Model identifier

        Returns:
            True if successful
        """
        if not self.cache.has(model_id):
            return False

        self.cache.remove(model_id)

        # Free CUDA memory
        if self.device == "cuda" and torch.cuda.is_available():
            torch.cuda.empty_cache()

        return True

    def get_memory_info(self) -> Dict[str, int]:
        """
        Get CUDA memory information

        Returns:
            Dictionary with memory info in MB
        """
        if self.device == "cuda" and torch.cuda.is_available():
            return {
                "allocated": int(torch.cuda.memory_allocated() / (1024 ** 2)),
                "reserved": int(torch.cuda.memory_reserved() / (1024 ** 2)),
                "max_allocated": int(torch.cuda.max_memory_allocated() / (1024 ** 2)),
                "total": int(torch.cuda.get_device_properties(0).total_memory / (1024 ** 2))
            }
        return {"allocated": 0, "reserved": 0, "max_allocated": 0, "total": 0}


# Global model runner instance
_model_runner: Optional[ModelRunner] = None


def initialize(device: str = "cuda") -> bool:
    """Initialize the model runner"""
    global _model_runner
    try:
        _model_runner = ModelRunner(device)
        return True
    except Exception as e:
        logger.error(f"Failed to initialize: {e}")
        return False


def load_model(model_path: str, model_id: str, model_type: str, precision: str = "fp16") -> bool:
    """Load a model"""
    if _model_runner is None:
        logger.error("ModelRunner not initialized")
        return False

    if model_type == "text_to_image":
        return _model_runner.load_text_to_image_model(model_path, model_id, precision)

    logger.error(f"Unsupported model type: {model_type}")
    return False


def generate_image(config_json: str) -> str:
    """Generate image from JSON config"""
    if _model_runner is None:
        return json.dumps({"success": False, "error": "ModelRunner not initialized"})

    try:
        config_dict = json.loads(config_json)
        config = InferenceConfig(**config_dict)
        result = _model_runner.generate_image(config)

        # Convert result to JSON
        result_dict = {
            "success": result.success,
            "error_message": result.error_message,
            "inference_time": result.inference_time,
            "memory_used": result.memory_used
        }

        # Convert image to base64 if present
        if result.image_data is not None:
            import base64
            from io import BytesIO
            from PIL import Image

            img = Image.fromarray(result.image_data)
            buffer = BytesIO()
            img.save(buffer, format="PNG")
            img_str = base64.b64encode(buffer.getvalue()).decode()
            result_dict["image_base64"] = img_str
            result_dict["image_width"] = result.image_data.shape[1]
            result_dict["image_height"] = result.image_data.shape[0]

        return json.dumps(result_dict)

    except Exception as e:
        logger.error(f"Error in generate_image: {e}")
        return json.dumps({"success": False, "error": str(e)})


def unload_model(model_id: str) -> bool:
    """Unload a model"""
    if _model_runner is None:
        return False
    return _model_runner.unload_model(model_id)


def get_memory_info() -> str:
    """Get memory info as JSON"""
    if _model_runner is None:
        return json.dumps({})
    return json.dumps(_model_runner.get_memory_info())


if __name__ == "__main__":
    # Test the model runner
    print("AI Forge Studio - Model Runner Test")
    print("=" * 50)

    if initialize():
        print("✓ ModelRunner initialized")
        print(f"Memory info: {get_memory_info()}")
    else:
        print("✗ Failed to initialize ModelRunner")
